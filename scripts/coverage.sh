#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="${repo_root}/build-coverage"
coverage_dir="${build_dir}/coverage"

cmake -S "${repo_root}" -B "${build_dir}" -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLIMIT_COVERAGE=ON
cmake --build "${build_dir}"

compiler_path="$(grep -E "^CMAKE_CXX_COMPILER:FILEPATH=" "${build_dir}/CMakeCache.txt" | cut -d= -f2)"
compiler_id="GCC"
if [[ "${compiler_path}" == *clang* ]]; then
  compiler_id="Clang"
fi

mkdir -p "${coverage_dir}"

if [[ "${compiler_id}" == "Clang" ]]; then
  if ! command -v llvm-profdata >/dev/null 2>&1 || ! command -v llvm-cov >/dev/null 2>&1; then
    echo "llvm-profdata/llvm-cov not found. Install llvm tools or rerun with GCC." >&2
    exit 1
  fi

  export LLVM_PROFILE_FILE="${coverage_dir}/limit-%p.profraw"
  ctest --test-dir "${build_dir}"
  llvm-profdata merge -sparse "${coverage_dir}"/*.profraw -o "${coverage_dir}/coverage.profdata"
  llvm-cov report "${build_dir}/limit-tests" -instr-profile="${coverage_dir}/coverage.profdata"
else
  find "${build_dir}/CMakeFiles/limit-tests.dir" -name "*.gcda" -delete || true
  ctest --test-dir "${build_dir}"
  if command -v gcovr >/dev/null 2>&1; then
    gcov_exe="gcov"
    if command -v gcov-14 >/dev/null 2>&1; then
      gcov_exe="gcov-14"
    fi
    mapfile -d '' objects < <(find "${build_dir}/CMakeFiles/limit-tests.dir/src" \
      "${build_dir}/CMakeFiles/limit-tests.dir/tests" -name "*.o" -print0)

    if [[ ${#objects[@]} -eq 0 ]]; then
      echo "No test object files found for coverage." >&2
      exit 1
    fi

    existing_gcov_files="$(mktemp)"
    new_gcov_files="$(mktemp)"
    find "${repo_root}" -maxdepth 1 -name "*.gcov" -print0 > "${existing_gcov_files}"

    pushd "${repo_root}" >/dev/null
    for obj in "${objects[@]}"; do
      obj_dir="$(dirname "${obj}")"
      "${gcov_exe}" -o "${obj_dir}" "${obj}" >/dev/null
    done

    gcovr -r "${repo_root}" \
      --use-gcov-files "${repo_root}" \
      --filter ".*src/.*" --filter ".*tests/.*" \
      --exclude ".*third_party/.*" \
      --exclude ".*_deps/.*" \
      --gcov-ignore-errors=all \
      --print-summary
    popd >/dev/null

    find "${repo_root}" -maxdepth 1 -name "*.gcov" -print0 > "${new_gcov_files}"
    while IFS= read -r -d '' file; do
      if ! grep -Fxz "${file}" "${existing_gcov_files}" >/dev/null; then
        rm -f "${file}"
      fi
    done < "${new_gcov_files}"

    rm -f "${existing_gcov_files}" "${new_gcov_files}"
  else
    echo "gcovr not found. Install gcovr for a coverage report (no sudo here)." >&2
    exit 1
  fi
fi

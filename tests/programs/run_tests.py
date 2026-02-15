#!/usr/bin/env python3
"""
End-to-end test runner for the Tiny compiler.

For each .tiny file, compiles it, runs the result, and compares stdout
against the corresponding .expected file.
"""

import argparse
import os
import subprocess
import sys
import tempfile
from pathlib import Path

def find_test_pairs(test_dir: Path):
    """Yield (tiny_path, expected_path) pairs."""
    for tiny_file in sorted(test_dir.rglob("*.tiny")):
        expected = tiny_file.with_suffix(".expected")
        if expected.exists():
            yield tiny_file, expected

def run_test(compiler: str, runtime: str, tiny_path: Path, expected_path: Path) -> bool:
    name = tiny_path.relative_to(tiny_path.parents[2])
    expected_output = expected_path.read_text()

    with tempfile.TemporaryDirectory() as tmp:
        ll_file = os.path.join(tmp, "output.ll")
        obj_file = os.path.join(tmp, "output.o")
        exe_file = os.path.join(tmp, "program")

        # Step 1: Compile .tiny → .ll
        result = subprocess.run(
            [compiler, str(tiny_path), "-o", ll_file],
            capture_output=True, text=True, timeout=10,
        )
        if result.returncode != 0:
            # For error tests, compare compiler stderr
            if "errors/" in str(tiny_path):
                actual = result.stderr.strip()
                if actual == expected_output.strip():
                    print(f"  PASS  {name} (expected error)")
                    return True
                else:
                    print(f"  FAIL  {name}")
                    print(f"        Expected error: {expected_output.strip()}")
                    print(f"        Got:            {actual}")
                    return False
            print(f"  FAIL  {name} (compilation failed)")
            print(f"        {result.stderr.strip()}")
            return False

        # Step 2: .ll → .o → executable
        subprocess.run(["llc", "-filetype=obj", ll_file, "-o", obj_file],
                       check=True, capture_output=True, timeout=10)
        subprocess.run(["clang", obj_file, runtime, "-o", exe_file],
                       check=True, capture_output=True, timeout=10)

        # Step 3: Run and compare
        result = subprocess.run([exe_file], capture_output=True, text=True, timeout=10)
        actual = result.stdout

    if actual.strip() == expected_output.strip():
        print(f"  PASS  {name}")
        return True
    else:
        print(f"  FAIL  {name}")
        print(f"        Expected: {expected_output.strip()!r}")
        print(f"        Got:      {actual.strip()!r}")
        return False

def main():
    parser = argparse.ArgumentParser(description="Tiny compiler end-to-end tests")
    parser.add_argument("--compiler", default="./build/tinyc", help="Path to tinyc binary")
    parser.add_argument("--runtime", default="./build/runtime/libtiny_runtime.a",
                        help="Path to runtime library")
    args = parser.parse_args()

    test_dir = Path(__file__).parent
    pairs = list(find_test_pairs(test_dir))

    if not pairs:
        print("No test pairs found. Add .tiny + .expected files to tests/programs/")
        return 0

    print(f"Running {len(pairs)} end-to-end tests...")
    print("=" * 50)

    passed = sum(1 for p in pairs if run_test(args.compiler, args.runtime, *p))
    failed = len(pairs) - passed

    print("=" * 50)
    print(f"Results: {passed} passed, {failed} failed, {len(pairs)} total")

    return 0 if failed == 0 else 1

if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
import subprocess
import os
import argparse

IRBuild_ptn = "{} -emit-ir {} -o {} {}"
ExeGen_ptn = '"clang" "-O0" "-o" "{}" "{}" "../lib/lib.c"'
Exe_ptn = '"{}"'


def eval(EXE_PATH: str, TEST_BASE_PATH: str, opts: str, testcases: dict[str, bool]):
    print("===========TEST START===========")
    print("now in {}".format(TEST_BASE_PATH))
    dir_succ = True
    max_len = max(len(case) for case in testcases)
    for case, need_input in testcases.items():
        print(f"Case \x1b[1m{case:<{max_len}}\x1b[0m", end="")
        TEST_PATH = os.path.join(TEST_BASE_PATH, case)
        SY_PATH = os.path.join(TEST_BASE_PATH, case + ".sy")
        LL_PATH = os.path.join(TEST_BASE_PATH, case + ".ll")
        INPUT_PATH = os.path.join(TEST_BASE_PATH, case + ".in")
        OUTPUT_PATH = os.path.join(TEST_BASE_PATH, case + ".out")

        IRBuild_result = subprocess.run(
            IRBuild_ptn.format(EXE_PATH, opts, LL_PATH, SY_PATH),
            shell=True,
            stderr=subprocess.PIPE,
        )
        if IRBuild_result.returncode == 0:
            input_option = None
            if need_input:
                with open(INPUT_PATH, "rb") as fin:
                    input_option = fin.read()

            try:
                subprocess.run(
                    ExeGen_ptn.format(TEST_PATH, LL_PATH),
                    shell=True,
                    stderr=subprocess.PIPE,
                )
                result = subprocess.run(
                    Exe_ptn.format(TEST_PATH),
                    shell=True,
                    input=input_option,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                )
                out = result.stdout.split(b"\n")
                if result.returncode != b"":
                    out.append(str(result.returncode).encode())
                for i in range(len(out) - 1, -1, -1):
                    out[i] = out[i].strip(b"\r")
                    if out[i] == b"":
                        out.remove(b"")
                case_succ = True
                with open(OUTPUT_PATH, "rb") as fout:
                    i = 0
                    for line in fout.readlines():
                        line = line.strip(b"\r").strip(b"\n")
                        if line == "":
                            continue
                        if out[i] != line:
                            dir_succ = False
                            case_succ = False
                        i = i + 1
                    if case_succ:
                        print("\t\033[32mPass\033[0m")
                    else:
                        print("\n".join(list(map(lambda x: x.decode(), out))))
                        print("\t\033[31mWrong Answer\033[0m")
            except Exception as _:
                dir_succ = False
                print(_, end="")
                print("\t\033[31mCodeGen or CodeExecute Fail\033[0m")
            finally:
                subprocess.call(["rm", "-rf", TEST_PATH, TEST_PATH])
                subprocess.call(["rm", "-rf", TEST_PATH, TEST_PATH + ".o"])
                subprocess.call(["rm", "-rf", TEST_PATH, TEST_PATH + ".ll"])

        else:
            dir_succ = False
            print("\t\033[31mIRBuild Fail\033[0m")
            subprocess.call(["rm", "-rf", TEST_PATH, TEST_PATH + ".ll"])
    if dir_succ:
        print("\t\033[32mSuccess\033[0m in dir {}".format(TEST_BASE_PATH))
    else:
        print("\t\033[31mFail\033[0m in dir {}".format(TEST_BASE_PATH))

    print("============TEST END============")


if __name__ == "__main__":
    # you can only modify this to add your testcase
    TEST_DIRS = [
        "./Test/Easy",
        "./Test/Medium",
        "./Test/Hard",
    ]
    # you can only modify this to add your testcase
    parser = argparse.ArgumentParser(description="Test script")
    parser.add_argument("-O2", action="store_true", help="Enable all optimizations")
    parser.add_argument(
        "-O", action="store_true", help="Enable Mem2Reg and DominateTree"
    )
    parser.add_argument("-check", action="store_true", help="Enable checker")
    parser.add_argument(
        "-lv", action="store_true", help="Enable live variable analysis"
    )
    parser.add_argument(
        "-cse", action="store_true", help="Enable common subexpression elimination"
    )
    args = parser.parse_args()
    opts: list[str] = []

    if args.O2:
        opts.append("-O2")
    if args.O:
        opts.append("-O")
    if args.check:
        opts.append("-check")
    if args.lv:
        opts.append("-lv")
    if args.cse:
        opts.append("-cse")
    for TEST_BASE_PATH in TEST_DIRS:
        testcases: dict[str, bool] = {}  # { name: need_input }
        EXE_PATH = os.path.abspath("../build/compiler")
        testcase_list = list(map(lambda x: x.split("."), os.listdir(TEST_BASE_PATH)))
        testcase_list.sort()
        for i in range(len(testcase_list)):
            testcases[testcase_list[i][0]] = False
        for i in range(len(testcase_list)):
            testcases[testcase_list[i][0]] = testcases[testcase_list[i][0]] | (
                testcase_list[i][1] == "in"
            )
        eval(EXE_PATH, TEST_BASE_PATH, " ".join(opts), testcases)

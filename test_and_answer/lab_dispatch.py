#!/usr/bin/env python3
import os
import sys

username = os.getlogin()

BASE_DIR = "/home/"+username+"/miniC"


def judge(cmd: str, testcase: int) -> str:
    res = os.popen(cmd+' '+str(testcase))
    res = res.read()
    print("testcase: "+str(testcase))
    print("res: " + res)
    return res


def lab1(task_id: str) -> str:
    task_list = {"judge_101.sh": 2, "judge_102.sh": 2, "judge_103.sh": 1,
                 "judge_104.sh": 5, "judge_105.sh": 6, "judge_106.sh": 1, "judge_107.sh": 1}
    if task_list.get(task_id):
        res = ""
        for testcase in range(0, task_list[task_id]):
            res = res + \
                judge("bash " + BASE_DIR +
                      "/test_and_answer/lab1/" + task_id, testcase)
        if res == "":
            res = "success"
        else:
            res = "Fail\n" + res
    else:
        res = "lab1 judge run error"
    return res


def lab2(task_id: str) -> str:
    task_list = {"judge_201.sh": 3, "judge_202.sh": 6, "judge_203.sh": 2}
    if task_list.get(task_id):
        res = ""
        for testcase in range(0, task_list[task_id]):
            res = res + \
                judge("bash " + BASE_DIR +
                      "/test_and_answer/lab2/" + task_id, testcase)
        if res == "":
            res = "success"
        else:
            res = "Fail\n" + res
    else:
        res = "lab2 judge run error"
    return res


def lab3(task_id: str) -> str:
    task_list = {"judge_301.sh": 1, "judge_302.sh": 2,
                 "judge_303.sh": 3, "judge_304.sh": 2, "judge_305.sh": 7}
    if task_list.get(task_id):
        res = ""
        for testcase in range(0, task_list[task_id]):
            res = res + \
                judge("bash " + BASE_DIR +
                      "/test_and_answer/lab3/" + task_id, testcase)
        if res == "":
            res = "success"
        else:
            res = "Fail\n" + res
    else:
        res = "lab3 judge run error"
    return res


def lab4(task_id: str) -> str:
    task_list = {"judge_401.sh": 3, "judge_402.sh": 3,
                 "judge_403.sh": 4, "judge_404.sh": 7, "judge_405.sh": 4}
    if task_list.get(task_id):
        res = ""
        for testcase in range(0, task_list[task_id]):
            res = res + \
                judge("bash " + BASE_DIR +
                      "/test_and_answer/lab4/" + task_id, testcase)
        if res == "":
            res = "success"
        else:
            res = "Fail\n" + res
    else:
        res = "lab4 judge run error"
    return res


def lab5(task_id: str) -> str:
    task_list = {"judge_501.sh": 1}
    if task_list.get(task_id):
        res = ""
        for testcase in range(0, task_list[task_id]):
            res = res + \
                judge("bash " + BASE_DIR +
                      "/test_and_answer/lab5/" + task_id, testcase)
        if res == "":
            res = "success"
        else:
            res = "Fail\n" + res
    else:
        res = "lab5 judge run error"
    return res

def update(lab_id:str):
    res = os.popen("cd /home/ubuntu/miniC;git pull")
    return res.read()

def lab_dispatch(lab_id: str):
    lab_map = {"lab1": lab1, "lab2": lab2,
               "lab3": lab3, "lab4": lab4, "lab5": lab5,"update":update}

    return lab_map[lab_id]


if __name__ == "__main__":
    lab_id = sys.argv[1]
    task_id = sys.argv[2]
    lab_func = lab_dispatch(lab_id)
    res = lab_func(task_id)
    print(res)

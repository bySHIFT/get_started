# get_started
 less is more


## 快速开始
需求:
- Windows 10 或 Ubuntu18.04
- Visual Studio 2015 Update 3 或更新的版本 (Windows 中)
- Git
- CMake 3.10

如何开始:
```SHELL
> git clone https://github.com/byshift/get_started.git
> cd get_started
> cmake -H. -Bbuild && cmake --build ./build
> ./build/src/cicd/cicd
> ./build/src/bingo/bingo
```

## 运行
```
============================================================
== 放胆开怀, 尽情肆意
== 2021-01-28 10:45:30 +0800
>> commit sha1: ce42bf7a
  job name: cppcheck
  stage: 1
  tag: B283454C-CD6E-55D8-0B92-7AF73FF809D1
  START: 2021-01-28 10:45:30 143506 +0800

....
>> commit sha1: ce42bf7a
  job name: build
  stage: 2
  tag: AB671B47-3F1E-4B1B-BF78-9009D648458D
  START: 2021-01-28 10:45:34 204035 +0800

.....
>> commit sha1: ce42bf7a
  job name: ci
  stage: 4
  tag: CA71F719-19E3-98C8-CBC9-8105D2B5FE7B
  START: 2021-01-28 10:45:40 233369 +0800

.....
>> commit sha1: ce42bf7a
  job name: cd
  stage: 8
  tag: 98646A3A-04E0-E07C-B99A-961DB70533B7
  START: 2021-01-28 10:45:46 277194 +0800

..........
>> commit sha1: ce42bf7a
  job name: clean
  stage: 16
  tag: DFB3B209-FBB9-2139-45F5-368D4EA99219
  START: 2021-01-28 10:45:57 307727 +0800

..........

>> STATUS: DONE

>> EXIT: 0
============================================================
== 放胆开怀, 尽情肆意
== 2021-01-28 10:46:08 +0800
(01) 01 13 14 21 29 31, 09
(02) 03 04 06 18 28 29, 16
(03) 06 11 12 29 30 32, 16
(04) 13 14 18 27 28 30, 01
(05) 14 18 23 24 26 32, 16

========================================
a3362#01131421293109#03040618282916#06111229303216#13141827283001#14182324263216
```

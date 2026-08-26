<!--
 * @Author: Trollface_巨魔
 * @Date: 2026-08-26
 * @Description: SymbolLang (~!@#$%) 语言项目主页
-->

<p align="center">
  <h1 align="center">~!@#$%</h1>
  <p align="center">
    <strong>始于 Word，不止于 Word</strong>
    <br>
    一门基于栈的符号驱动型编程语言
    <br>
    <a href="https://github.com/Trollface1145/SymbolLang"><strong>探索仓库 »</strong></a>
    <br>
    <br>
    <a href="#-快速开始">快速开始</a>
    ·
    <a href="#-语法概览">语法概览</a>
    ·
    <a href="#-示例程序">示例</a>
    ·
    <a href="#-贡献">贡献</a>
  </p>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=c%2B%2B" alt="C++17">
  <img src="https://img.shields.io/badge/license-Creative%20Gears-red?style=flat-square" alt="License">
  <img src="https://img.shields.io/badge/version-0.4.3-brightgreen?style=flat-square" alt="Version">
  <img src="https://img.shields.io/badge/PRs-welcome-brightgreen?style=flat-square" alt="PRs Welcome">
</p>

---

##  简介

**~!@#$%**（别名 **X X XXX**）是一门基于栈的符号驱动型编程语言，由 **Trollface1145** 于 2026 年暑假设计并实现。  
它通过符号形状暗示操作含义，所有指令以 `@` 开头，表示“当前册（作用域）的操作”。语言面向有编程基础的用户，同时通过“方言”机制降低初学者的学习门槛。

> 本项目从 **Word VBA** 起步，现已移植为独立 C++ 解释器，支持 Windows / Linux / macOS。

---

##  特性

-  **符号指令** – 所有操作以 `@` 开头，简洁一致
-  **多栈操作** – 支持显式指定目标栈（`<@ '栈名'`）
-  **变量与类型** – 支持整型、浮点型、布尔型（`0.4.2` 起）
-  **指针操作** – 支持创建、赋值、解引用（`0.4.3` 起）
-  **控制流** – `if`、`else`、`while`、`for`
-  **函数定义** – 支持参数与返回值
-  **输入输出** – `@>` 输出，`@<` 输入
-  **方言机制** – 支持中文方言（`#<chinese_library>`），降低学习门槛
-  **跨平台** – C++17 实现，可编译为独立 `.exe` 运行

---

##  快速开始

### 1. 编译解释器

确保系统已安装 **C++17 编译器**（MinGW、MSVC 或 Clang）。

```bash
# 使用 MinGW / GCC
g++ -std=c++17 -o interpreter.exe interpreter.cpp
```

```bash
# 使用 MSVC
cl /EHsc /std:c++17 interpreter.cpp /Fe:interpreter.exe
```

### 2.写一个Hello World!吧
创建.~文件，并输入以下代码
```
@>"Hello World!"
```
保存，把文件拖放到IDE.bat，回车运行

-----
### 语法概览
#### 基础指令

| 指令        | 功能       | 示例        |
|-----------|----------|-----------|
| @! 栈名     | 创建栈      | @! X      |
| @*~ 值<变量  | 压栈并绑定变量  | @*~ 10<a  |
| @!!~ 表达式  | 计算表达式，压栈 | @!!~ a+b  |
| @&& 栈名    | 输出栈顶     | @&& X     |
| @>"文本"    | 输出文本或表达式 | @>"计算结果：" |
| @<"提示"<变量 | 输入数字     | @<"请输入"<n |

#### 布尔与指针
| 指令        | 功能     | 示例         |
|-----------|--------|------------|
| @# 变量=值   | 声明布尔变量 | @# flag=*# |
| !!@ 指针名   | 创建指针   | !!@ p      |
| @& 指针名<变量 | 指针指向变量 | @& p<a     |
| @* 指针名    | 解引用指针  | @> @* p    |

#### 贡献
欢迎任何形式的贡献！

请遵循标准 GitHub 流程：

Fork 本仓库

创建你的功能分支 (git checkout -b feature/AmazingFeature)

提交改动 (git commit -m 'Add some AmazingFeature')

推送到分支 (git push origin feature/AmazingFeature)

打开一个 Pull Request

#### 许可证
本项目采用 Creative Gears 专属许可证，保留所有权利。
未经作者书面许可，不得用于商业用途。

© 2014-2026 Creative Gears. All rights reserved.

#### 致谢
作者：Trollface_巨魔（QQ: 2166439786）

特别感谢：DeepSeek 在技术文档与代码调试中的协助

灵感来源：Phigros 判定线、RKS 机制、栈机语言思想

<p align="center"> <sub>始于 Word，不止于 Word —— 2026 年 8 月，一个准初中生用一整个暑假，让创意变成了现实。</sub> </p> ```
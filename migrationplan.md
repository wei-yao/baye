Here is Claude's plan:
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
 三国霸业 Qt5 C++ → Godot 4 GDScript 迁移计划

 Context

 这是一个基于 Qt5 C++ 的 2D
 三国策略游戏（"天际边三国霸业"），原始引擎来自嵌入式掌机平台（159×96
 像素虚拟屏幕），后移植到 Qt 支持 Windows/Android/iOS。代码约 34,000 行，140+
 文件。目标是迁移到 Godot 4 + GDScript，使其可在 macOS 上运行，并保留跨平台能力。

 ---
 阶段 0：环境搭建 + 资源提取（第 1-2 周）

 依赖安装

 - Godot Engine 4.3+：从 godotengine.org 下载 macOS 版本 + 导出模板
 - Python 3.10+：编写资源提取脚本
 - Git LFS：管理 res.rlb (~50MB) 等大文件

 资源提取

 编写 Python 脚本，移植 datman.cpp 中的二进制解析逻辑：

 1. dat.lib (192KB) → JSON 数据文件
   - 格式：索引表(每 ResId 4 字节偏移) + RCHEAD(12 字节:
 ResLen/ResId/ItmCnt/ItmLen/ResKey) + 数据
   - 解密：ptr[i] -= ResKey
   - 输出：persons.json, cities.json, goods.json, skills.json, strings.json
  update tools/extract_datlib.py created
 2. res.rlb (~50MB) → PNG 精灵图
   - 图片头 PictureHeadType(6 字节: wid/hig/count/mask)
   - 1-bit 图：每字节 8 像素，参考 BytesToHBitmap
   - 彩色/JPG：参考 BytesToHBitmap_Color / BytesToHBitmap_JPG
 3. font.bin → 不迁移，直接用 DroidSansFallback.ttf
 4. 字符串 GB18030 → UTF-8 转换

 关键源文件：datman.cpp, fsys.cpp, PicConverter.cpp/h

 验证

 - 提取的 PNG 与原版渲染截图对比
 - 武将(200)、城市(38)、技能(29) 数据量和数值交叉验证

 ---
 阶段 1：核心数据模型 + 主菜单（第 3-4 周）

 Godot 项目结构

 baye_godot/
 ├── project.godot
 ├── assets/
 │   ├── sprites/          # 提取的 PNG（generals/, tiles/, units/, skills/, ui/)
 │   ├── fonts/            # DroidSansFallback.ttf
 │   ├── buttons/          # 25 PNG 按钮
 │   └── data/             # JSON 数据文件
 ├── scripts/
 │   ├── autoload/
 │   │   ├── game_data.gd  # 全局数据（替代 extern 全局变量）
 │   │   ├── game_state.gd # 状态机（替代 ng/nga/ngb/ngc）
 │   │   └── config.gd     # 配置管理
 │   ├── core/
 │   │   ├── person.gd     # PersonType → Resource
 │   │   ├── city.gd       # CityType → Resource
 │   │   ├── order.gd      # OrderType → Resource
 │   │   └── constants.gd  # consdef.h + fight.h 常量
 │   ├── strategy/         # 城市管理、AI 策略
 │   ├── battle/           # 战斗系统
 │   ├── ui/               # UI 组件
 │   └── util/             # 存档、资源加载
 ├── scenes/               # tscn 场景文件
 └── tools/                # Python 离线工具

 数据模型

 - person.gd：PersonType (Id, Belong, Level, Force, IQ, Devotion, Character, Experience,
  Thew, ArmsType, Arms, Equip, Age)
 - city.gd：CityType (Id, Belong, SatrapId, Farming, Commerce, Population, Money, Food,
 PersonV, ToolsV, Links[8])
 - order.gd：OrderType (OrderId, Person, City, Object, Arms, Food, Money, Consume,
 TimeCount)
 - constants.gd：PERSON_MAX=200, CITY_MAX=38, ORDER_MAX=100 等

 关键源文件：attribute.h, order.h, consdef.h, gamEng.cpp, global.cpp/h

 主菜单

 - 新游戏 / 读档 / 关于
 - 时期选择（4 个历史时期）→ 君主选择

 验证

 - 游戏可启动，主菜单流程走通
 - 4 个时期的初始数据正确加载

 ---
 阶段 2：战略地图 + 城市管理（第 5-7 周）

 战略地图

 - 城市地图 tile → Godot TileMapLayer
 - 城市图标 + 选择光标 → Sprite2D
 - 12×9 城市地图布局

 城市指令系统（18+ 指令）

 - 内政：开垦、招商、搜寻、视察、厚赐、处刑...
 - 外交：离间、游说、反间、诱降...
 - 军事：征兵、侦察、掠夺、出征...
 - 三级菜单 UI（类别 → 指令 → 武将选择）

 回合驱动

 - 月循环：玩家指令 → AI 指令 → 条件更新（收入/粮食/灾害）
 - ComputerTactic() → ai_tactic.gd

 存档系统

 - 新格式用 JSON，支持导入旧二进制存档

 关键源文件：citycmd.cpp ~ citycmde.cpp, tactic.cpp, showface.cpp, savechange.cpp

 验证

 - 38 城市地图位置和连接关系正确
 - 每个城市指令数值变化与原版一致
 - AI 回合正常执行
 - 月更新（收入、人口、粮食）数值正确
 - 存档/读档数据完整

 ---
 阶段 3：战斗系统（第 8-10 周）

 战斗地图

 - 32×32 格子 → TileMapLayer（16×16 tile，8 种地形）
 - 每方最多 10 将

 战斗单位

 - JLPOS (x, y, hp, mp, move, active, state) → battle_unit.gd
 - 6 兵种：骑(移5)、步(移4)、弓(移4)、水(移5)、极(移6)、玄(移3)
 - 兵种克制关系

 移动 + 攻击

 - 移植 CountMoveP/FgtCountPath 扩散寻路算法
 - 攻击范围 5×5，伤害公式：hurt = (攻/防) * (兵力>>3) * 克制系数 + 10

 技能系统

 - 29 种技能，各有目标类型、伤害、状态效果、天气/地形修正
 - 技能范围 9×9
 - 8 种状态效果（混乱、禁咒、定身等）

 战斗 AI

 - 移植 FgtPkAi.cpp：近距离目标选择 + 技能评估

 关键源文件：Fight.cpp, FightSub.cpp, FgtCount.cpp, FgtPkAi.cpp, fight.h

 验证

 - 单场战斗可完整进行（胜利/失败/逃跑）
 - 兵种移动力、克制伤害正确
 - 29 种技能效果正确
 - AI 正常移动和攻击

 ---
 阶段 4：UI 完善 + 特效（第 11-12 周）

 - 战斗动画播放（AnimatedSprite2D）
 - 消息框系统
 - 配置菜单（键位、画面、速度）
 - 触控适配（Godot InputEvent 天然支持）

 ---
 阶段 5：网络/MOD（可选，第 13-14 周）

 - TCP 存档同步 → HTTPRequest + REST API
 - MOD 系统 → Godot PCK 导出
 - 用户登录

 ---
 风险点

 ┌─────────────────────────────────┬──────┬─────────────────────────────────────────┐
 │              风险               │ 级别 │                  应对                   │
 ├─────────────────────────────────┼──────┼─────────────────────────────────────────┤
 │ dat.lib/res.rlb 二进制解析      │ 高   │ Python 提取器与 C++                     │
 │                                 │      │ 运行时结果逐字节对比                    │
 ├─────────────────────────────────┼──────┼─────────────────────────────────────────┤
 │ 159×96 虚拟屏幕渲染差异         │ 高   │ SubViewport + 最近邻缩放保持像素风格    │
 ├─────────────────────────────────┼──────┼─────────────────────────────────────────┤
 │ 城市指令系统复杂度(5            │ 高   │ 逐指令迁移，每个独立测试                │
 │ 文件数千行)                     │      │                                         │
 ├─────────────────────────────────┼──────┼─────────────────────────────────────────┤
 │ 全局状态管理(30+ extern 变量)   │ 中   │ autoload 单例 + 枚举状态机重构          │
 ├─────────────────────────────────┼──────┼─────────────────────────────────────────┤
 │ 战斗寻路算法                    │ 中   │ 先 1:1 移植，后续可用 Godot AStar2D     │
 │                                 │      │ 优化                                    │
 ├─────────────────────────────────┼──────┼─────────────────────────────────────────┤
 │ 旧存档兼容                      │ 低   │ 支持导入但新存档用 JSON                 │
 └─────────────────────────────────┴──────┴─────────────────────────────────────────┘

 GDExtension 需求

 预计不需要。离线转换资源后，所有运行时逻辑纯 GDScript 可胜任。
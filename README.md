# wxPriceQuery-Origin

* [English Version](#English-Version)
* [简体中文版](#简体中文版)
* [繁體中文版](#繁體中文版)

English Version
====

## Overview

If you had once purchased games from Origin, you may realised the prices are different in differen countries / regions. But usually the Origin will only show the prices of the player's region.

This tool can queries and shows you all the prices of all the regions where Origin supported. And then it will convert the currency and sort the list for each item you've selected.

This application is built with wxWidgets in order to provides coress-platform binaries. Unfortunately I can only provide the binary file of Windows version as I don't have an Apple comnputer.

The built-in multiple language translation support of wxWidgets is not used here because I want to make sure the binary file is just a single file. I made a simple solution instead.

### Supported Languages

* English
* Chinese (Simplified)
* Chinese (Tradition)

## How to Compile

C++ standard: C++17

### Dependencies

* nlohmann-json
* rapidxml
* wxWdigets (≥ 3.1.5)

#### Windows

The best way for Windows users is download and use [vcpkg](https://github.com/microsoft/vcpkg) to manage the dependent packages.

* Visual Studio 2019+ is recommended
* Install the dependent packages with Static linking option

#### macOS

Please open ‘*.workspace’ file with CodeLite and complie the project after all dependent packages are configured.

I don't have an Apple Computer, so I can't test and build it in macOS.

#### BSD / Linux

Please open ‘*.workspace’ file with CodeLite and complie the project after all dependent packages are configured.

## APIs of Exchange rates
Two APIs are used in this project:
* European Celtral Bank: [Euro foreign exchange reference rates](https://www.ecb.europa.eu/stats/policy_and_exchange_rates/euro_reference_exchange_rates/html/index.en.html)
* [ExchangeRate-API](https://www.exchangerate-api.com/docs/free)

---

简体中文版
====

## 概述

如果你在 Origin 平台购买过游戏，或许会知道不同国家/地区会有不同价格，有些区域价格较高，有些价格较低。通常情况下，Origin 只会显示玩家所在地区的价格列表。

本工具可以直接显示 Orign 所有货币区的价格，同时做货币转换，再对不同的价格、区域做排序显示。

这个程序使用了 wxWidgets 以便能够获得跨平台的能力。可惜的是，我没有苹果电脑，只能提供 Windows 版本的二进制文件。

另外，wxWidgets 自带的多语言翻译功能在这里并未使用，因为这样就无法提供单一的二进制文件。于是我单独做了个简易方案来代替。


### 目前支持的语言

* 简体中文
* 繁体中文
* 英文

## 编译

C++标准：C++17

### 依赖项

* nlohmann-json
* rapidxml
* wxWdigets (≥ 3.1.5)

#### Windows

Windows 用户强烈建议使用 vcpkg 安装依赖项

* 推荐使用 Visual Studio 2019+
* 安装依赖包时使用静态编译

#### macOS

配置好依赖项后，可使用 CodeLite 打开 workspace 文件开始编译。

我没有苹果电脑，因此无法在 macOS 内测试。

#### BSD / Linux

配置好依赖项后，可使用 CodeLite 打开 workspace 文件开始编译。

## 使用到的汇率 API
目前使用以下两个汇率 API:
* European Celtral Bank: [Euro foreign exchange reference rates](https://www.ecb.europa.eu/stats/policy_and_exchange_rates/euro_reference_exchange_rates/html/index.en.html)
* [ExchangeRate-API](https://www.exchangerate-api.com/docs/free)

---

繁體中文版
====

## 概述

如果你在 Origin 平臺購買過遊戲，或許會知道不同國家/地區會有不同價格，有些區域價格較高，有些價格較低。通常情況下，Origin 只會顯示玩家所在地區的價格列表。

本工具可以直接顯示 Orign 所有貨幣區的價格，同時做貨幣轉換，再對不同的價格、區域做排序顯示。

這個程式使用了 wxWidgets 以便能夠獲得跨平臺的能力。可惜的是，我沒有蘋果電腦，只能提供 Windows 版本的二進制檔。

另外，wxWidgets 自帶的多語言翻譯功能在這裡並未使用，因爲這樣就無法提供單一的二進制檔。於是我單獨做了個簡易方案來代替。


### 目前支持的語言

* 簡體中文
* 繁體中文
* 英文

## 編譯

C++標準：C++17

### 依賴項

* nlohmann-json
* rapidxml
* wxWdigets (≥ 3.1.5)

#### Windows

Windows 用戶強烈建議使用 vcpkg 安裝依賴項

* 推薦使用 Visual Studio 2019+
* 安裝依賴包時使用靜態編譯

#### macOS

配置好依賴項後，可使用 CodeLite 打開 workspace 文件開始編譯。

我沒有蘋果電腦，因此無法在 macOS 內測試。

#### BSD / Linux

配置好依賴項後，可使用 CodeLite 打開 workspace 文件開始編譯。

## 使用到的滙率 API
目前使用以下兩個滙率 API:
* European Celtral Bank: [Euro foreign exchange reference rates](https://www.ecb.europa.eu/stats/policy_and_exchange_rates/euro_reference_exchange_rates/html/index.en.html)
* [ExchangeRate-API](https://www.exchangerate-api.com/docs/free)
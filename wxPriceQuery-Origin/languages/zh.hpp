﻿#pragma once
#include "languages.hpp"

namespace languages
{
	struct language_chinese_s : public language_basic_class
	{
		language_chinese_s()
		{
			currency_full_name =
			{
				{"USD", "美元"},
				{"JPY", "日元"},
				{"BGN", "保加利亚列弗"},
				{"CZK", "捷克克朗"},
				{"DKK", "丹麦克朗"},
				{"EUR", "欧元"},
				{"GBP", "英镑"},
				{"HUF", "匈牙利福林"},
				{"PLN", "波兰兹罗提"},
				{"RON", "罗马尼亚列伊"},
				{"SEK", "瑞典克朗"},
				{"CHF", "瑞士法郎"},
				{"ISK", "冰岛克朗"},
				{"NOK", "挪威克朗"},
				{"HRK", "克罗地亚库纳"},
				{"RUB", "俄罗斯卢布"},
				{"TRY", "土耳其里拉"},
				{"AUD", "澳元"},
				{"BRL", "巴西雷亚尔"},
				{"CAD", "加拿大元"},
				{"CNY", "人民币"},
				{"HKD", "港元"},
				{"IDR", "印尼盾"},
				{"ILS", "以色列谢克尔"},
				{"INR", "印度卢比"},
				{"KRW", "韩元"},
				{"MXN", "墨西哥比索"},
				{"MYR", "马来西亚林吉特"},
				{"NZD", "新西兰元"},
				{"PHP", "菲律宾比索"},
				{"SGD", "新加坡元"},
				{"THB", "泰铢"},
				{"TWD", "新台币"},
				{"ZAR", "南非兰特"}
			};

			two_letter_country_to_full_name =
			{
				{"AU", "澳大利亚"},
				{"BE", "比利时"},
				{"BR", "巴西"},
				{"CA", "加拿大"},
				{"DE", "德意志"},
				{"DK", "丹麦"},
				{"ES", "西班牙"},
				{"FI", "芬兰"},
				{"FR", "法兰西"},
				{"GB", "大不列颠"},
				{"HK", "香港"},
				{"IE", "爱尔兰"},
				{"IN", "印度"},
				{"IT", "意大利"},
				{"JP", "日本"},
				{"KR", "韩国"},
				{"MX", "墨西哥"},
				{"NL", "尼德兰（荷兰）"},
				{"NO", "挪威"},
				{"NZ", "新西兰"},
				{"PL", "波兰"},
				{"RU", "俄罗斯"},
				{"SE", "瑞典"},
				{"SG", "新加坡"},
				{"TH", "泰国"},
				{"TW", "台湾"},
				{"US", "美国"},
				{"ZA", "南非"}
			};

			static_text_mapping =
			{
				{"app_title", "Origin 价格查询工具"},
				{"search", "搜索"},
				{"price_sorting", "价格排序"},
				{"supported_languages", "可用语言"},
				{"settings", "设置…"},
				{"interface_settings", "界面设置"},
				{"interface_language", "界面语言"},
				{"my_currency", "我的货币"},
				{"origin_parameters", "Origin 参数"},
				{"lng_game_list", "游戏列表显示语言:"},
				{"region", "Origin 地区:"},
				{"ok_btn", "确定"},
				{"cancel_btn", "取消"},
				{"catalogue", "分类"},
				{"title_name", "游戏名称"},
				{"item_type", "类型"},
				{"platform", "平台"},
				{"release_date", "发布日期"},
				{"on_sale", "正在促销"},
				{"wait", "请稍等"},
				{"download", "正在从 Origin 服务器下载数据"},
				{"connect", "正在连接到 Origin 服务器"},
				{"cannot_connect", "无法连接到 Origin 服务器"},
				{"message", "消息"},
				{"conversion_failed", "转换失败"},
				{"search_result", "搜索结果"},
				{"all_games", "所有游戏"},
				{"current_region", "当前区域:"},
				{"other_region", "其它区域:"},
				{"selection_missing", "有选项未选择"},
				{"select_currency", "请选择货币。"},
				{"select_language", "请选择语言。"},
				{"select_region", "请选择区域。"},
				{"other_genre", "其它类别"},
				{"other_franchise", "其它品牌"},
				{"other_game_type", "其它游戏类型"}
			};
		}
	};

	struct language_chinese_t : public language_basic_class
	{
		language_chinese_t()
		{
			currency_full_name =
			{
				{"USD", "美元"},
				{"JPY", "日元"},
				{"BGN", "保加利亞列弗"},
				{"CZK", "捷克克朗"},
				{"DKK", "丹麥克朗"},
				{"EUR", "歐元"},
				{"GBP", "英鎊"},
				{"HUF", "匈牙利福林"},
				{"PLN", "波蘭茲羅提"},
				{"RON", "羅馬尼亞列伊"},
				{"SEK", "瑞典克朗"},
				{"CHF", "瑞士法郎"},
				{"ISK", "冰島克朗"},
				{"NOK", "挪威克朗"},
				{"HRK", "克羅地亞庫納"},
				{"RUB", "俄羅斯盧布"},
				{"TRY", "土耳其里拉"},
				{"AUD", "澳元"},
				{"BRL", "巴西雷亞爾"},
				{"CAD", "加拿大元"},
				{"CNY", "人民幣"},
				{"HKD", "港元"},
				{"IDR", "印尼盾"},
				{"ILS", "以色列謝克爾"},
				{"INR", "印度盧比"},
				{"KRW", "韩元"},
				{"MXN", "墨西哥比索"},
				{"MYR", "馬來西亞林吉特"},
				{"NZD", "新西蘭元"},
				{"PHP", "菲律賓比索"},
				{"SGD", "新加坡元"},
				{"THB", "泰銖"},
				{"TWD", "新台幣"},
				{"ZAR", "南非蘭特"}
			};

			two_letter_country_to_full_name =
			{
				{"AU", "澳大利亞"},
				{"BE", "比利時"},
				{"BR", "巴西"},
				{"CA", "加拿大"},
				{"DE", "德意志"},
				{"DK", "丹麥"},
				{"ES", "西班牙"},
				{"FI", "芬蘭"},
				{"FR", "法蘭西"},
				{"GB", "大不列顛"},
				{"HK", "香港"},
				{"IE", "愛爾蘭"},
				{"IN", "印度"},
				{"IT", "意大利"},
				{"JP", "日本"},
				{"KR", "韓國"},
				{"MX", "墨西哥"},
				{"NL", "尼德蘭（荷蘭）"},
				{"NO", "挪威"},
				{"NZ", "新西蘭"},
				{"PL", "波瀾"},
				{"RU", "俄羅斯"},
				{"SE", "瑞典"},
				{"SG", "新加坡"},
				{"TH", "泰國"},
				{"TW", "臺灣"},
				{"US", "美國"},
				{"ZA", "南非"}
			};

			static_text_mapping =
			{
				{"app_title", "Origin 價格查詢工具"},
				{"search", "搜尋"},
				{"price_sorting", "價格排序"},
				{"supported_languages", "可用語言"},
				{"settings", "設定…"},
				{"interface_settings", "介面設定"},
				{"interface_language", "介面語言"},
				{"my_currency", "我的貨幣"},
				{"origin_parameters", "Origin 參數"},
				{"lng_game_list", "遊戲列表顯示語言:"},
				{"region", "Origin 地區:"},
				{"ok_btn", "確定"},
				{"cancel_btn", "取消"},
				{"catalogue", "分類"},
				{"title_name", "遊戲名稱"},
				{"item_type", "類型"},
				{"platform", "平台"},
				{"release_date", "發布日期"},
				{"on_sale", "正在促銷"},
				{"wait", "請稍等"},
				{"download", "正在從 Origin 伺服器下載資料"},
				{"connect", "正在連接到 Origin 伺服器"},
				{"cannot_connect", "無法連接到 Origin 伺服器"},
				{"message", "消息"},
				{"conversion_failed", "轉換失敗"},
				{"search_result", "搜尋結果"},
				{"all_games", "所有遊戲"},
				{"current_region", "當前區域:"},
				{"other_region", "其它區域:"},
				{"selection_missing", "有選項未選擇" },
				{"select_currency", "請選擇貨幣。"},
				{"select_language", "請選擇語言。"},
				{"select_region", "請選擇區域。"},
				{"other_genre", "其它類別"},
				{"other_franchise", "其它品牌"},
				{"other_game_type", "其它遊戲類型"}

			};
		}
	};
}

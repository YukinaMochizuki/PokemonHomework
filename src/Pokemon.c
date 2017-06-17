/*
 ============================================================================
 Name        : Pokemon.c
 Author      : Yukina
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>

//函式原型宣告
int readConfiguration(char source_system[60]);
int operatingStateManageSystem(char source_system[60],char *game_state,int type);

//宣告資料結構

struct checkStructIsInitialized_data{
	int variable_number;
	char **variable_name;
	int *variable_detail;
};

struct person_finalhomework{
	char class[64];
	int studentID;
	char name[64];
	char date[64];
	char phone_number[64];
};

struct Lowlevel_area_data{
	char name[30];
//	是否為初始城鎮
	int initial_town;
//	地區類型，1城鎮、2草原、3城市
	int area_type;
//	子地區數目
	int subordinate_area_num;
//	子地圖
	char **subordinate_area;
//	特殊房屋數目
	int special_house_num;
//	特殊房屋
	char **special_house;
//	神奇寶貝數目
	int pokemon_list_num;
//	神奇寶貝列表
	char **pokemon_list;
	char **pokemon_level;
	int NPC_house_num;
	char **NPC_house;
	int NPC_list_num;
	char **NPC_list;
	int special_items_num;
	char **special_items;
};

struct Lowlevel_basic_attributes{
	char name[30];
	int attribute_basic_bonus_num;
	char **attribute_basic_bonus_name;
	int *attribute_basic_bonus;
};

struct Lowlevel_ability_value{
	int HP;
	int attack;
	int defense;
	int mana_attack;
	int mana_defense;
	int evasion_rate;
	int speed;
	int luck;
};

struct Lowlevel_pokemon_data{
	char name[30];
//	等級曲線
	int grading_curve_type;
//	屬性數量，默認為零
	int attributes_num;
//	屬性類型
	char **attributes_type;
//	基礎能力值
	struct Lowlevel_ability_value base_ability_value;
//	加成能力值
	struct Lowlevel_ability_value ability_value_add_table;
//	種族招式表數量
	int racial_skills_table_num;
//	種族招式表
	char **racial_skills_table;
//	種族招式獲得等級
	int *racial_skills_level;
//	可學會的所有招式數量
	int skills_table_num;
//	可學會的所有招式
	char **skills_table;
};

struct Lowlevel_skill_data{
	char name[30];
//	屬性分類數量 (與Pokemon屬性有關)
	int classification_attribute_num;
//	屬性分類 (與Pokemon屬性有關)
	char **classification_attribute;
//	技能擁有屬性數量(與Pokenmon屬性無關)
	int attribute_num;
//	技能擁有屬性(與Pokenmon屬性無關)
	char **attribute;
//	招式威力(攻擊力加成)(接受小數點)
	double skill_power;
//	命中率
	int accuracy;
//	詠唱回合(預設為0)
	int chant_round;
//	調用基本事件器
	int call_base_event_system_num;
	char **call_base_event_system;
};

struct Lowlevel_NPC_data{
	char name[30];
//	屬性
	int attribute_num;
	char **attribute;
//	調用基本事件器
	int call_base_event_system_num;
	char **call_base_event_system;
};

struct Lowlevel_props_data{
	char name[30];
	int attribute_num;
	char **attribute;
	int disposable_props;
//	調用基本事件器
	int call_base_event_system_num;
	char **call_base_event_system;
};

struct Lowlevel_trigger_event_data{
	char name[64];
//	在原野或戰鬥狀態
	int environment;
//
	char event_listener[64];
	char event_listener_conditions[64];

	int triggering_conditions_num;
	char **triggering_conditions;
	char **triggering_conditions_variableCA;
	char **triggering_conditions_variableCB;
	int *triggering_conditions_variableIA;
	int *triggering_conditions_variableIB;

	int triggering_event_num;
	char **triggering_event;
	char **triggering_event_variableCA;
	char **triggering_event_variableCB;
	int *triggering_event_variableIA;
	int *triggering_event_variableIB;

};

void exitf(void){
}

void getTime(char *time_return){
//	char *time_return = (char *) malloc(sizeof(char) * 15);
//	char time_return[15];
//	???
	time_t total_seconds;
//	time_t是一種變數型態，它是一長整數long int，用於儲存時間之總秒數
	total_seconds = time(NULL);
//	從某時刻到現在的時間，傳入NULL則return當前的總秒數
	int sec, min, hour;
	total_seconds += 8 * 60 * 60;
	sec = total_seconds % 60;
	min = (total_seconds / 60) % 60;
	hour = (total_seconds / 3600) % 24;
	sprintf(time_return, "[%02d:%02d:%02d]", hour, min, sec);
//    %2d兩格不補零，%02d兩格補零
//    stdio.h的格式化函數，與printf使用方法相同，唯獨第一個引數為輸出結果
//    return time_return;
}

void getLogger(int massage_type,char source_system[60],char message[80]){
	char severity_level[10];
	static int non_debug = 0;
	switch(massage_type){
	case 0:
		sprintf(severity_level, "info");
//		直接用=賦值會發生
//		Description Resource Path Location Type assignment to expression with array type
//		目前不知道為什麼..= =
		break;
	case 1:
		sprintf(severity_level, "debug");
		if(non_debug)return;
		break;
	case 2:
		sprintf(severity_level, "warn");
		break;
	case 3:
		sprintf(severity_level, "error");
		break;
	case 10:
		non_debug = 1;
		return;
	}
	char get_time[15];
	char *now_time = get_time;
	getTime(now_time);
	printf("%s [%s] [%s]: %s\n", now_time, severity_level, source_system, message);
	fflush(stdout);
}

//初始化狀態時使用，初始化之後的狀態呼叫不起作用
//用於簡化錯誤訊息的輸出
int initializationErrorMessager(char source_system[60],char event_system[60]){
	char this_system[60] = "callAndPreprocessingSystem";
	char logger_message[80];
	int severity_level = 0;

	char error_return[30];
	char game_state[30];
	char *get_game_state = game_state;
	int type = 0;

	if(operatingStateManageSystem(this_system, get_game_state, type) == 0){
		if(strcmp(game_state, "PRE_INITIALIZATION") == 0){
			severity_level = 2;
			sprintf(logger_message, "Loading config %s is warn", event_system);
			getLogger(severity_level, source_system, logger_message);
			sprintf(logger_message,
					"Would you like to continue reading the setup information? (Y/n)");
			getLogger(severity_level, source_system, logger_message);

			gets(error_return);
			if(strcmp(error_return, "Y") == 0){
				return -1;
			}else if(strcmp(error_return, "n") == 0){
				return 0;
			}else{
				severity_level = 3;
				sprintf(logger_message, "Input string does not match requirements");
				getLogger(severity_level, source_system, logger_message);
				sprintf(logger_message, "The program is being stopped");
				getLogger(severity_level, source_system, logger_message);
				return -1;
			}
		}else if(strcmp(game_state, "INITIALIZATION") == 0){

		}else if(strcmp(game_state, "POST_INITIALIZATION") == 0){

		}else if(strcmp(game_state, "LOAD_COMPLETE") == 0){

		}

	}

	return 0;
}

//運行事件管理器，來源系統僅用於辨認是否由main呼叫，無其他關連輸出
//自定義規範:
//觸發PRE_INITIALIZATION時，系統層級的模塊應該在這個階段開始引用各自的配置文件
//觸發INITIALIZATION時，系統應準備好基礎的交互訊息，並且建立或驗證本身所需的依賴
//觸發POST_INITIALIZATION時，系統應準備好完整的交互介面，事件監聽器及事件觸發器應在此時初始化完成
//觸發LOAD_COMPLETE時，所有系統均準備就緒
int operatingStateManageSystem(char source_system[60],char *game_state,int type){
	char this_system[] = "operatingStateManageSystem";
	char logger_message[80];
	int severity_level = 0;

	static char game_state_record[60];
	static int game_state_record_int = 0;

	static char source_system_name_record[30][60];
	static int source_system_call_number_record[30];
	static int source_system_number_record = -1;

//	切換運行狀態，由main負責
	if(type == 1 && strcmp(source_system, "main") == 0){
		if(game_state_record_int == 0){
			game_state_record_int++;
			sprintf(game_state_record, "PRE_INITIALIZATION");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

//			初始化系統呼叫次數紀錄
			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}else if(game_state_record_int == 1){
			game_state_record_int++;
			sprintf(game_state_record, "INITIALIZATION");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}else if(game_state_record_int == 2){
			game_state_record_int++;
			sprintf(game_state_record, "POST_INITIALIZATION");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}else if(game_state_record_int == 3){
			game_state_record_int++;
			sprintf(game_state_record, "LOAD_COMPLETE");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}else if(game_state_record_int == 4){
			game_state_record_int++;
			sprintf(game_state_record, "SERVER_ABOUT_TO_START");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}else if(game_state_record_int == 5){
			game_state_record_int++;
			sprintf(game_state_record, "SERVER_STARTING");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}else if(game_state_record_int == 6){
			game_state_record_int++;
			sprintf(game_state_record, "SERVER_STOPPING");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}else if(game_state_record_int == 7){
			game_state_record_int++;
			sprintf(game_state_record, "SERVER_STOPPED");

			severity_level = 1;
			sprintf(logger_message, "Now game state is %s", game_state_record);
			getLogger(severity_level, this_system, logger_message);

			for(int m = 0; m <= 30; m++){
				source_system_call_number_record[m] = -1;
			}
			return 0;
		}
	}else if(type == 1){
		severity_level = 2;
		sprintf(logger_message,
				"Source call system error,pls use the main function to switch the game state");
		getLogger(severity_level, this_system, logger_message);
	}

//	回傳運行狀態
	if(type == 0 && game_state_record_int != 0){
		sprintf(game_state, "%s", game_state_record);

		if(source_system_number_record == -1){
			source_system_number_record++;
			sprintf(source_system_name_record[source_system_number_record], "%s", source_system);
			source_system_call_number_record[source_system_number_record]++;
			return source_system_call_number_record[source_system_number_record];
		}else{
			for(int m = 0; m <= 30; m++){
				if(strcmp(source_system_name_record[source_system_number_record], source_system)
						== 0){
					source_system_call_number_record[m]++;
					return source_system_call_number_record[m];
				}
			}
			source_system_number_record++;
			sprintf(source_system_name_record[source_system_number_record], "%s", source_system);
			source_system_call_number_record[source_system_number_record]++;
			return source_system_call_number_record[source_system_number_record];
		}
	}else if(type == 0 && game_state_record_int == 0){
		severity_level = 2;
		sprintf(logger_message, "The game state is not initialized");
		getLogger(severity_level, this_system, logger_message);
		return -1;
	}
	return -1;
}

//檢查結構是否初始化
//主要用途：傳入變數名稱和模式後，根據模式將變數名稱對應的retrun疊加和讀取，
//　    　　　　在傳入name引數內容時將其底下的變數全數重置，
//		 initialization傳入時將所有記憶體重置準備接受下一個系統的調用。
int checkStructIsInitialized(char source_system[60],char Name[60],int type){
	char this_system[] = "CSIISystem";
	char logger_message[80];
	int severity_level = 0;

	static int source_system_record_number = 0;
	static int now_variable_record = -1; ////這個要記得重置時一併重置
	static int variable_amount_record = -1;
	static struct checkStructIsInitialized_data CSIIsystem_data;

	char name[] = "name";
	char initialization[] = "initialization";

//	初始化來源系統紀錄
	if(strcmp(Name, initialization) == 0){
		if(source_system_record_number != 0){
			for(int m = 0; m < variable_amount_record; m++)
				free(CSIIsystem_data.variable_name[m]);
			free(CSIIsystem_data.variable_name);
			free(CSIIsystem_data.variable_detail);
			now_variable_record = -1;
//		這裡要釋放上一次結構使用的所有記憶體空間
		}
		source_system_record_number++;
		variable_amount_record = type;

		if(variable_amount_record == -1){
			severity_level = 3;
			sprintf(logger_message, "The variable_amount_record is not initialization for %s",
					source_system);
			getLogger(severity_level, this_system, logger_message);
			return -1;
		}

		severity_level = 0;
		sprintf(logger_message, "The CSIIsystem is ready to initialize the configuration %s",
				source_system);
		getLogger(severity_level, this_system, logger_message);

//		初始化variable_name字串陣列
		CSIIsystem_data.variable_name = malloc(sizeof(char*) * variable_amount_record);
		for(int m = 0; m < variable_amount_record; m++){
			CSIIsystem_data.variable_name[m] = malloc(sizeof(char) * 64);
		}

		severity_level = 1;
		sprintf(logger_message, "%d string arrays have been initialized for use",
				variable_amount_record);
		getLogger(severity_level, this_system, logger_message);

//		初始化variable_amount_record陣列
		CSIIsystem_data.variable_detail = (int *)malloc(sizeof(int) * variable_amount_record);

		severity_level = 1;
		sprintf(logger_message, "%d int arrays have been initialized for use",
				variable_amount_record);
		getLogger(severity_level, this_system, logger_message);
		return 0;
	}

//	遇到Name時將變數資料重置
	if(strcmp(Name, name) == 0){
		for(int m = 0; m < variable_amount_record; m++){
			CSIIsystem_data.variable_detail[m] = -1;
		}
		return 0;
	}

//	紀錄變數細節
	int compare_variables = 0;
	int compare_variables_number = 0;
	for(int m = 0; m < variable_amount_record; m++){
		if(strcmp(CSIIsystem_data.variable_name[m], Name) == 0){
			compare_variables = 1;
			compare_variables_number = m;
		}
	}
	if(compare_variables == 0){
		now_variable_record++;
		sprintf(CSIIsystem_data.variable_name[now_variable_record], "%s", Name);
		CSIIsystem_data.variable_detail[now_variable_record]++;

		severity_level = 1;
		sprintf(logger_message, "The variable_name is %s", Name);
		getLogger(severity_level, this_system, logger_message);

		severity_level = 1;
		sprintf(logger_message, "The variable initialize number is %d",
				CSIIsystem_data.variable_detail[now_variable_record]);
		getLogger(severity_level, this_system, logger_message);

		return CSIIsystem_data.variable_detail[now_variable_record];
	}else if(compare_variables == 1){
		CSIIsystem_data.variable_detail[compare_variables_number]++;

		severity_level = 1;
		sprintf(logger_message, "The variable_name is %s", Name);
		getLogger(severity_level, this_system, logger_message);

		severity_level = 1;
		sprintf(logger_message, "The variable initialize number is %d",
				CSIIsystem_data.variable_detail[compare_variables_number]);
		getLogger(severity_level, this_system, logger_message);

		return CSIIsystem_data.variable_detail[compare_variables_number];
	}
//------------------------------------------------------------------------

	/* 以下廢棄
	 //	宣告結構陣列
	 static struct checkStructIsInitialized_data checkStructIsInitialized_data[20];


	 //	確定系統編號及基本初始化
	 if(register_system_number == -1){
	 //	初始化checkStructIsInitialized系統結構陣列
	 checkStructIsInitialized_data[0].name = "CSIISystemData";
	 register_system_number ++;
	 }
	 if(register_system_number == 0){
	 now_system_number = 1;
	 register_system_number++;
	 sprintf(checkStructIsInitialized_data[now_system_number].name,"%s",source_system);
	 checkStructIsInitialized_data[now_system_number].variable_number = 0;

	 }else{
	 for(int number = 0;number <= register_system_number;number++){
	 if(!strcmp(checkStructIsInitialized_data[number].name,source_system)){
	 now_system_number = number;
	 check_system_number = 0;
	 break;
	 }
	 }
	 if(check_system_number){
	 register_system_number++;
	 now_system_number = register_system_number - 1;
	 sprintf(checkStructIsInitialized_data[now_system_number].name,"%s",source_system);
	 checkStructIsInitialized_data[now_system_number].variable_number = 0;
	 }
	 }

	 //	確認系統變數編號
	 if(strcmp(Name,name) == 0){

	 }else if(checkStructIsInitialized_data[now_system_number].variable_number == 0){
	 now_variable_number = 0;
	 checkStructIsInitialized_data[now_system_number].variable_number++;
	 sprintf(checkStructIsInitialized_data[now_system_number].variable_name[now_variable_number][20],"%s",Name);
	 }else{
	 for(int number = 0;number < checkStructIsInitialized_data[now_system_number].variable_number;number++){
	 if(!strcmp(checkStructIsInitialized_data[now_system_number].variable_name[number][20],Name)){
	 now_variable_number = number;
	 check_variable_number = 0;
	 break;
	 }
	 }
	 if(check_variable_number){
	 checkStructIsInitialized_data[now_system_number].variable_number++;
	 now_variable_number = checkStructIsInitialized_data[now_system_number].variable_number - 1;
	 sprintf(checkStructIsInitialized_data[now_system_number].variable_name[now_variable_number][20],"%s",Name);
	 }
	 }

	 //	開始讀入參數
	 //	type 0累加 1讀入模式
	 if(type){
	 if(!strcmp(Name,name)){
	 if(checkStructIsInitialized_data[now_system_number].variable_number == 0){

	 }

	 }




	 }else if(type == 0){

	 }else{

	 }



	 static char *variable_name[25];
	 static int **variable_detail = NULL;
	 int add_an_array = 1;

	 //	結構陣列項目初始值為-1
	 if(Name == -1){
	 severity_level = 3;
	 sprintf(logger_message,"The name is not initialized");
	 getLogger(severity_level,source_system,logger_message);
	 //		回傳-1值判定發生嚴重錯誤
	 return -1;
	 }

	 //	確定系統編號
	 if(register_system_number ==0){
	 now_system_number = register_system_number;
	 register_system_number[now_system_number] =
	 source_system;
	 register_system_number++;

	 }else{
	 for(int num = 0;num <= register_system_number;num++){
	 if(strcmp(register_system_number[num],source_system == 0)){
	 now_system_number = num;
	 add_an_array = 0;
	 break;
	 }
	 }
	 if(add_an_array){
	 now_system_number = register_system_number;
	 register_system_number[register_system_number] =
	 source_system;
	 register_system_number++;
	 }
	 }*/

	return -1;
}

/*int initializeParameterManagementSystem(char source_system[10],char variable_name[20],int *variable_detail,int variable_initialize){
 //	return 0 成功設置變數 1 變數已經初始化
 //	variable_initialize  -1
 static char *source_system_record[10];

 static int source_system_record_number;


 //	開始檢查系統和變數內容
 for(source_system_record_number,)

 }*/

int basicAttributesSystem(char config_item[64],char config_variable[64],int call_type,char *callback_variable){
	char this_system[] = "basicAttributesSystem";
	char logger_message[80];
	int severity_level = 0;

//	獲取運行狀態
	char game_state[30];
	char *get_game_state = game_state;
	int eventManageSystem_type = 0;
	int call_record = operatingStateManageSystem(this_system, get_game_state,
			eventManageSystem_type);
	if(call_record == -1){
		severity_level = 3;
		sprintf(logger_message, "An error occurred while calling the Event Manage System");
		getLogger(severity_level, this_system, logger_message);
		return -1;
	}

//	計算讀取資料個數
	static int basic_attributes_record = 0;
	char number_of_reading[20] = "number of loadings";
//	checkStructIsInitialized所需要的變數數目
	int CSIItype = 2;
//	宣告結構指標
	static struct Lowlevel_basic_attributes *Lowlevel_basic_attributes_entity;

	if(strcmp(get_game_state, "PRE_INITIALIZATION") == 0){
//		運行狀態中被第一次呼叫
		if(call_record == 0){
			if(readConfiguration("area_data_system") == -1){
				if(initializationErrorMessager(this_system, "area_data_system") == -1) return -1;
			}
		}
		if(basic_attributes_record == 0){
			if(strcmp(config_item, number_of_reading) == 0 && atoi(config_variable) != 0){
//				atoi為標準庫之char轉int函式，如果無法轉換則回傳0
				basic_attributes_record = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "There are %d settings that will be loaded",
						basic_attributes_record);
				getLogger(severity_level, this_system, logger_message);

//				初始化結構
				Lowlevel_basic_attributes_entity = (struct Lowlevel_basic_attributes*)malloc(
						basic_attributes_record * sizeof(struct Lowlevel_basic_attributes));

//				初始化checkStructIsInitialized函式
				char initialization[] = "initialization";
				checkStructIsInitialized(this_system, initialization, CSIItype);
				return 0;
			}else{
				severity_level = 3;
				sprintf(logger_message, "No number of reading is find");
				getLogger(severity_level, this_system, logger_message);
				return -1;
			}
		}

		char name[] = "name";
		char attribute_basic_bonus_num[] = "attribute_basic_bonus_num";
		char attribute_basic_bonus_name[] = "attribute_basic_bonus_name";
		char attribute_basic_bonus[] = "attribute_basic_bonus";

//		開始輸入資料
		static int now_struct_num = -1;

		if(strcmp(config_item, name) == 0){
			now_struct_num++;
			sprintf(Lowlevel_basic_attributes_entity[now_struct_num].name, "%s", config_variable);

			severity_level = 1;
			sprintf(logger_message, "The name_variable is %s be loaded", config_variable);
			getLogger(severity_level, this_system, logger_message);

//			結構初始化
			Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_num = -1;

//			呼叫初始化函式
			severity_level = 1;
			sprintf(logger_message, "The Initialize system will be loading");
			getLogger(severity_level, this_system, logger_message);

			checkStructIsInitialized(this_system, name, CSIItype);

		}else if(strcmp(config_item, attribute_basic_bonus_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return -1;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute_basic_bonus_num %d is done",
						atoi(config_variable));
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The attribute_basic_bonus_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return -1;
			}
		}else if(strcmp(config_item, attribute_basic_bonus_name) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return -1;
			}
			if(Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The attribute_basic_bonus_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message,
						"Failed to loading suattribute basic bonus namebordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return -1;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_name = malloc(
						sizeof(char*)
								* Lowlevel_basic_attributes_entity[now_struct_num]
										.attribute_basic_bonus_num);
				for(int m = 0;
						m
								< Lowlevel_basic_attributes_entity[now_struct_num]
										.attribute_basic_bonus_num; m++){
					Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_name[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The attribute_basic_bonus_name char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(
						Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_name[0],
						"%s", config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute_basic_bonus_name variable %s is done",
						config_variable);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(
						Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_name[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message,
						"The subordinatLowlevel_basic_attributes_entitye_area variable %s is done",
						config_variable);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, attribute_basic_bonus) == 0){
//			int*
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return -1;
			}
			if(Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The attribute_basic_bonus_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading attribute basic bonus setting");
				getLogger(severity_level, this_system, logger_message);
				return -1;
			}

//			準備輸入
			if(atoi(config_variable) != 0){
				int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
				if(CSIInum == 0){
//					初始化
					Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus = (int *)malloc(
							sizeof(int)
									* Lowlevel_basic_attributes_entity[now_struct_num]
											.attribute_basic_bonus_num);
					severity_level = 1;
					sprintf(logger_message,
							"The attribute_basic_bonus char array memory space allocated");
					getLogger(severity_level, this_system, logger_message);

//					第一筆資料
					Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus[0] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The attribute_basic_bonus variable %s is done",
							config_variable);
					getLogger(severity_level, this_system, logger_message);
				}else if(CSIInum == -1){
//					初始化錯誤
					severity_level = 3;
					sprintf(logger_message, "The checkStructIsInitialized system is warn");
					getLogger(severity_level, this_system, logger_message);
				}else{
					Lowlevel_basic_attributes_entity[now_struct_num].attribute_basic_bonus[CSIInum] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The attribute_basic_bonus variable %s is done",
							config_variable);
					getLogger(severity_level, this_system, logger_message);
				}
			}
		}else{
			severity_level = 2;
			sprintf(logger_message, "No variables found to meet the requirements");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "Please check if there are variable errors ");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "or annotations set the wrong situation");
			getLogger(severity_level, this_system, logger_message);
		}
	}
	return 0;
}

char** areaDataSystem(char config_item[64],char config_variable[64],int call_type,char *callback_variable,
		char **incoming_struct,char source_system[60]){
	char this_system[] = "areaDataSystem";
	char logger_message[80];
	int severity_level = 0;

//	配置系統運行狀況報告
	char *system_statue_ok = "ok";
	char *system_statue_error = "error";
	char **this_system_statue = &system_statue_error;

//	獲取運行狀態
	char game_state[30];
	char *get_game_state = game_state;
	int eventManageSystem_type = 0;
	int call_record = operatingStateManageSystem(this_system, get_game_state,eventManageSystem_type);
	if(call_record == -1){
		severity_level = 3;
		sprintf(logger_message, "An error occurred while calling the Operating State Manage System");
		getLogger(severity_level, this_system, logger_message);

		return this_system_statue = &system_statue_error;
	}

//	計算讀取資料個數
	static int area_data_record = 0;
	char number_of_reading[20] = "number of loadings";
//	紀錄目前已讀取項目及讀取檔案設定進度
//	PRE_INITIALIZATION狀態之後請勿更動此數值
	static int now_struct_num = -1;
//	checkStructIsInitialized所需要的變數數目
	int CSIItype = 8;
//	宣告結構指標
	static struct Lowlevel_area_data *Lowlevel_area_data_entity;

	if(strcmp(get_game_state, "PRE_INITIALIZATION") == 0){
//		運行狀態中被第一次呼叫
		if(call_record == 0){
			if(readConfiguration("area_data_system") == -1){
				return this_system_statue = &system_statue_error;
			}else{
				return this_system_statue = &system_statue_ok;
			}
		}
		if(area_data_record == 0){
			if(strcmp(config_item, number_of_reading) == 0 && atoi(config_variable) != 0){
//				atoi為標準庫之char轉int函式，如果無法轉換則回傳0
				area_data_record = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "There are %d settings that will be loaded",area_data_record);
				getLogger(severity_level, this_system, logger_message);

//				初始化結構
				Lowlevel_area_data_entity = (struct Lowlevel_area_data*)malloc(
						area_data_record * sizeof(struct Lowlevel_area_data));

//				初始化checkStructIsInitialized函式
				char initialization[] = "initialization";
				checkStructIsInitialized(this_system, initialization, CSIItype);
				return 0;
			}else{
				severity_level = 3;
				sprintf(logger_message, "No number of reading is find");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}

		char name[] = "name";
		char initial_town[] = "initial_town";
		char area_type[] = "area_type";
		char subordinate_area_num[] = "subordinate_area_num";
		char subordinate_area[] = "subordinate_area";
		char special_house_num[] = "special_house_num";
		char special_house[] = "special_house";
		char pokemon_list_num[] = "pokemon_list_num";
		char pokemon_list[] = "pokemon_list";
		char pokemon_level[] = "pokemon_level";
		char NPC_house_num[] = "NPC_house_num";
		char NPC_house[] = "NPC_house";
		char NPC_list_num[] = "NPC_list_num";
		char NPC_list[] = "NPC_list";
		char special_items_num[] = "special_items_num";
		char special_items[] = "special_items";

//		開始輸入資料

		if(strcmp(config_item, name) == 0){
			now_struct_num++;
			sprintf(Lowlevel_area_data_entity[now_struct_num].name, "%s", config_variable);

			severity_level = 1;
			sprintf(logger_message, "The name_variable is %s be loaded", Lowlevel_area_data_entity[now_struct_num].name);
			getLogger(severity_level, this_system, logger_message);

//			結構初始化
			Lowlevel_area_data_entity[now_struct_num].initial_town = -1;
			Lowlevel_area_data_entity[now_struct_num].area_type = -1;
			Lowlevel_area_data_entity[now_struct_num].subordinate_area_num = -1;
			Lowlevel_area_data_entity[now_struct_num].special_house_num = -1;
			Lowlevel_area_data_entity[now_struct_num].pokemon_list_num = -1;
			Lowlevel_area_data_entity[now_struct_num].NPC_house_num = -1;
			Lowlevel_area_data_entity[now_struct_num].NPC_list_num = -1;
			Lowlevel_area_data_entity[now_struct_num].special_items_num = -1;

//			呼叫初始化函式
			severity_level = 1;
			sprintf(logger_message, "The Initialize system will be loading");
			getLogger(severity_level, this_system, logger_message);

			checkStructIsInitialized(this_system, name, CSIItype);

		}else if(strcmp(config_item, initial_town) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].initial_town = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The initial_town %d is done", Lowlevel_area_data_entity[now_struct_num].initial_town);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The initial_town value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, area_type) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading area type setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].area_type = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The area_type %d is done", Lowlevel_area_data_entity[now_struct_num].area_type);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The area_type value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, subordinate_area_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area number setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].subordinate_area_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The subordinate_area_num %d is done",
						Lowlevel_area_data_entity[now_struct_num].subordinate_area_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The subordinate_area_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, subordinate_area) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_area_data_entity[now_struct_num].subordinate_area_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The subordinate_area_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_area_data_entity[now_struct_num].subordinate_area = malloc(
						sizeof(char*)
								* Lowlevel_area_data_entity[now_struct_num].subordinate_area_num);
				for(int m = 0; m < Lowlevel_area_data_entity[now_struct_num].subordinate_area_num;
						m++){
					Lowlevel_area_data_entity[now_struct_num].subordinate_area[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The subordinate_area char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].subordinate_area[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The subordinate_area variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].subordinate_area[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].subordinate_area[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The subordinate_area variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].subordinate_area[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, special_house_num) == 0){
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading special house number setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].special_house_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The special_house_num %d is done", Lowlevel_area_data_entity[now_struct_num].special_house_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The special_house_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, special_house) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_area_data_entity[now_struct_num].special_house_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The special_house is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading special house setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_area_data_entity[now_struct_num].special_house = malloc(
						sizeof(char*)
								* Lowlevel_area_data_entity[now_struct_num].special_house_num);
				for(int m = 0; m < Lowlevel_area_data_entity[now_struct_num].special_house_num;
						m++){
					Lowlevel_area_data_entity[now_struct_num].special_house[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The special_house char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].special_house[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The special_house variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].special_house[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].special_house[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The special_house variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].special_house[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, pokemon_list_num) == 0){
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading special house number setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].pokemon_list_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The pokemon_list_num %d is done", Lowlevel_area_data_entity[now_struct_num].pokemon_list_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The pokemon_list_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, pokemon_list) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_area_data_entity[now_struct_num].pokemon_list_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The pokemon_list_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading pokemon list setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_area_data_entity[now_struct_num].pokemon_list = malloc(
						sizeof(char*) * Lowlevel_area_data_entity[now_struct_num].pokemon_list_num);
				for(int m = 0; m < Lowlevel_area_data_entity[now_struct_num].pokemon_list_num; m++){
					Lowlevel_area_data_entity[now_struct_num].pokemon_list[m] = malloc(
							sizeof(char) * 64);
				}
				severity_level = 1;
				sprintf(logger_message, "The pokemon_list char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].pokemon_list[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The pokemon_list variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].pokemon_list[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].pokemon_list[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The pokemon_list variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].pokemon_list[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, pokemon_level) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_area_data_entity[now_struct_num].pokemon_list_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The pokemon_list_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading pokemon list setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_area_data_entity[now_struct_num].pokemon_level = malloc(
						sizeof(char*) * Lowlevel_area_data_entity[now_struct_num].pokemon_list_num);
				for(int m = 0; m < Lowlevel_area_data_entity[now_struct_num].pokemon_list_num; m++){
					Lowlevel_area_data_entity[now_struct_num].pokemon_level[m] = malloc(
							sizeof(char) * 64);
				}
				severity_level = 1;
				sprintf(logger_message, "The pokemon_level char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].pokemon_level[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The pokemon_level variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].pokemon_level[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].pokemon_level[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The pokemon_level variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].pokemon_level[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, NPC_house_num) == 0){
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading NPC house number setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].NPC_house_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The NPC_house_num %d is done", Lowlevel_area_data_entity[now_struct_num].NPC_house_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The NPC_house_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, NPC_house) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading NPC house setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_area_data_entity[now_struct_num].NPC_house_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The NPC_house_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading NPC house setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_area_data_entity[now_struct_num].NPC_house = malloc(
						sizeof(char*) * Lowlevel_area_data_entity[now_struct_num].NPC_house_num);
				for(int m = 0; m < Lowlevel_area_data_entity[now_struct_num].NPC_house_num; m++){
					Lowlevel_area_data_entity[now_struct_num].NPC_house[m] = malloc(
							sizeof(char) * 64);
				}
				severity_level = 1;
				sprintf(logger_message, "The NPC_house char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].NPC_house[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The NPC_house variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].NPC_house[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].NPC_house[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The NPC_house variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].NPC_house[CSIInum]);
				getLogger(severity_level, this_system, logger_message);

			}
		}else if(strcmp(config_item, NPC_list_num) == 0){
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading NPC list number setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].NPC_list_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The NPC_list_num %d is done", atoi(config_variable));
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The NPC_list_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, NPC_list) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading NPC list setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_area_data_entity[now_struct_num].NPC_list_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The NPC_house_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading NPC list setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_area_data_entity[now_struct_num].NPC_list = malloc(
						sizeof(char*) * Lowlevel_area_data_entity[now_struct_num].NPC_list_num);
				for(int m = 0; m < Lowlevel_area_data_entity[now_struct_num].NPC_list_num; m++){
					Lowlevel_area_data_entity[now_struct_num].NPC_list[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The NPC_list char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].NPC_list[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The NPC_list variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].NPC_list[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].NPC_list[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The NPC_list variable %s is done", config_variable);
				getLogger(severity_level, this_system, logger_message);

			}
		}else if(strcmp(config_item, special_items_num) == 0){
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading special items number setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_area_data_entity[now_struct_num].special_items_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The special_items_num %d is done",
						Lowlevel_area_data_entity[now_struct_num].special_items_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The special_items_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, special_items) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading special items setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_area_data_entity[now_struct_num].special_items_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The special_items_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading special items setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_area_data_entity[now_struct_num].special_items = malloc(
						sizeof(char*)
								* Lowlevel_area_data_entity[now_struct_num].special_items_num);
				for(int m = 0; m < Lowlevel_area_data_entity[now_struct_num].special_items_num;
						m++){
					Lowlevel_area_data_entity[now_struct_num].special_items[m] = malloc(
							sizeof(char) * 64);
				}
				severity_level = 1;
				sprintf(logger_message, "The special_items char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].special_items[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The special_items variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].special_items[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_area_data_entity[now_struct_num].special_items[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The special_items variable %s is done",
						Lowlevel_area_data_entity[now_struct_num].special_items[CSIInum]);
				getLogger(severity_level, this_system, logger_message);

			}
		}else{
			severity_level = 2;
			sprintf(logger_message, "No variables found to meet the requirements");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "Please check if there are variable errors ");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "or annotations set the wrong situation");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "The variables is %s%s",config_item,config_variable);
			getLogger(severity_level, this_system, logger_message);
		}
	}else if(strcmp(get_game_state, "INITIALIZATION") == 0){
//		讀取
		if(call_type == 0){
			int find_name_record = 0;
			int find_name_num;
			for(int m=0;m <= now_struct_num;m++){
				if(strcmp(Lowlevel_area_data_entity[m].name,config_item)==0){
					find_name_record++;
					find_name_num = m;
					break;
				}
			}
			if(find_name_record == 0){
				severity_level = 2;
				sprintf(logger_message, "The Name variable %s can not be found",config_item);
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}else if(find_name_record == 1){
				if(strcmp("initial_town", config_variable) == 0
						|| strcmp("area_type", config_variable) == 0
						|| strcmp("subordinate_area_num", config_variable) == 0
						|| strcmp("special_house_num", config_variable) == 0
						|| strcmp("pokemon_list_num", config_variable) == 0
						|| strcmp("NPC_house_num", config_variable) == 0
						|| strcmp("NPC_list_num", config_variable) == 0
						|| strcmp("special_items_num", config_variable) == 0){
//					int
					if(strcmp("initial_town", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].initial_town);
					if(strcmp("area_type", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].area_type);
					if(strcmp("subordinate_area_num", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].subordinate_area_num);
					if(strcmp("special_house_num", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].special_house_num);
					if(strcmp("pokemon_list_num", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].pokemon_list_num);
					if(strcmp("NPC_house_num", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].NPC_house_num);
					if(strcmp("NPC_list_num", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].NPC_list_num);
					if(strcmp("special_items_num", config_variable) == 0)
						sprintf(callback_variable,"int.%d",Lowlevel_area_data_entity[find_name_num].special_items_num);

					severity_level = 1;
					sprintf(logger_message, "The name:%s variables:%s is be read %s for %s", config_item,
							config_variable, callback_variable, source_system);
					getLogger(severity_level, this_system, logger_message);
				}else if(strcmp("subordinate_area", config_variable) == 0
						|| strcmp("special_house", config_variable) == 0
						|| strcmp("pokemon_list", config_variable) == 0
						|| strcmp("pokemon_level", config_variable) == 0
						|| strcmp("NPC_house", config_variable) == 0
						|| strcmp("NPC_list", config_variable) == 0
						|| strcmp("special_items", config_variable) == 0){
//					**char
					if(strcmp("subordinate_area", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_area_data_entity[find_name_num].subordinate_area;
					}else if(strcmp("special_house", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_area_data_entity[find_name_num].special_house;
					}else if(strcmp("pokemon_list", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_area_data_entity[find_name_num].pokemon_list;
					}else if(strcmp("pokemon_level", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_area_data_entity[find_name_num].pokemon_level;
					}else if(strcmp("NPC_house", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_area_data_entity[find_name_num].NPC_house;
					}else if(strcmp("NPC_list", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_area_data_entity[find_name_num].NPC_list;
					}else if(strcmp("special_items", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_area_data_entity[find_name_num].special_items;
					}
				}else{
					severity_level = 2;
					sprintf(logger_message, "No variables found to meet the requirements");
					getLogger(severity_level, this_system, logger_message);

					return this_system_statue = &system_statue_error;
				}
			}
//		寫入
//		}else if(call_type == 1){
//			int find_name_record = 0;
//			int find_name_num;
//			for(int m=0;m < now_struct_num;m++){
//				if(strcmp(Lowlevel_area_data_entity[m].name,config_item)==0){
//					find_name_record++;
//					find_name_num = m;
//					break;
//				}
//			}
//			if(find_name_record == 0){
//				severity_level = 2;
//				sprintf(logger_message, "The Name variable %s can not be found",config_item);
//				getLogger(severity_level, this_system, logger_message);
//				return -1;
//			}
//			if(strcmp("initial_town", config_variable) == 0
//					|| strcmp("area_type", config_variable) == 0
//					|| strcmp("subordinate_area_num", config_variable) == 0
//					|| strcmp("special_house_num", config_variable) == 0
//					|| strcmp("pokemon_list_num", config_variable) == 0
//					|| strcmp("NPC_house_num", config_variable) == 0
//					|| strcmp("NPC_list_num", config_variable) == 0
//					|| strcmp("special_items_num", config_variable) == 0){
////				int
//				if(atoi(callback_variable) == 0){
//					severity_level = 2;
//					sprintf(logger_message, "The callback_variable value is warn");
//					getLogger(severity_level, this_system, logger_message);
//					return -1;
//				}
//				int variable_record = atoi(callback_variable);
//				if(strcmp("initial_town", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].initial_town = variable_record;
//				if(strcmp("area_type", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].area_type = variable_record;;
//				if(strcmp("subordinate_area_num", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].subordinate_area_num = variable_record;;
//				if(strcmp("special_house_num", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].special_house_num = variable_record;;
//				if(strcmp("pokemon_list_num", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].pokemon_list_num = variable_record;;
//				if(strcmp("NPC_house_num", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].NPC_house_num = variable_record;;
//				if(strcmp("NPC_list_num", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].NPC_list_num = variable_record;;
//				if(strcmp("special_items_num", config_variable) == 0)
//					Lowlevel_area_data_entity[find_name_num].special_items_num = variable_record;;
//
//				severity_level = 1;
//				sprintf(logger_message, "The name:%s variables:%s is be writed %s", config_item,
//						config_variable, callback_variable);
//				getLogger(severity_level, this_system, logger_message);
//			}else if(strcmp("subordinate_area", config_variable) == 0
//					|| strcmp("special_house", config_variable) == 0
//					|| strcmp("pokemon_list", config_variable) == 0
//					|| strcmp("pokemon_level", config_variable) == 0
//					|| strcmp("NPC_house", config_variable) == 0
//					|| strcmp("NPC_list", config_variable) == 0
//					|| strcmp("special_items", config_variable) == 0){
////				**char
//				if(strcmp("subordinate_area", config_variable) == 0){
//					sprintf(callback_variable,"char**.%d",
//							Lowlevel_area_data_entity[find_name_num].subordinate_area_num);
//					callback_variable2 = Lowlevel_area_data_entity[find_name_num].subordinate_area;
//				}else if(strcmp("special_house", config_variable) == 0){
//					sprintf(callback_variable, "char**.%d",
//							Lowlevel_area_data_entity[find_name_num].special_house_num);
//					callback_variable2 = Lowlevel_area_data_entity[find_name_num].special_house;
//				}else if(strcmp("pokemon_list", config_variable) == 0){
//					sprintf(callback_variable, "char**.%d",
//							Lowlevel_area_data_entity[find_name_num].pokemon_list_num);
//					callback_variable2 = Lowlevel_area_data_entity[find_name_num].pokemon_list;
//				}else if(strcmp("pokemon_level", config_variable) == 0){
//					sprintf(callback_variable,"char**.%d",
//							Lowlevel_area_data_entity[find_name_num].pokemon_list_num);
//					callback_variable2 = Lowlevel_area_data_entity[find_name_num].pokemon_level;
//				}else if(strcmp("NPC_house", config_variable) == 0){
//					sprintf(callback_variable,"char**.%d",
//							Lowlevel_area_data_entity[find_name_num].NPC_house_num);
//					callback_variable2 = Lowlevel_area_data_entity[find_name_num].NPC_house;
//				}else if(strcmp("NPC_list", config_variable) == 0){
//					sprintf(callback_variable,"char**.%d",
//							Lowlevel_area_data_entity[find_name_num].NPC_list_num);
//					callback_variable2 = Lowlevel_area_data_entity[find_name_num].NPC_list;
//				}else if(strcmp("special_items", config_variable) == 0){
//					sprintf(callback_variable,"char**.%d",
//							Lowlevel_area_data_entity[find_name_num].special_items_num);
//					callback_variable2 = Lowlevel_area_data_entity[find_name_num].special_items;
//				}
//
//				severity_level = 1;
//				sprintf(logger_message, "The name:%s char**_variables:%s is be read", config_item,
//						config_variable);
//			}
		}else if(call_type == 3){
//			驗證地區資料
			int initial_town_check = 0;
			for(int m = 0;m <= now_struct_num;m++){
				if(Lowlevel_area_data_entity[m].initial_town == 1 && initial_town_check ==1){
					severity_level = 3;
					sprintf(logger_message, "There is more than one initial town!");
					getLogger(severity_level, this_system, logger_message);
				}
				if(Lowlevel_area_data_entity[m].initial_town == 1)initial_town_check = 1;





			}















		}
	}

	/*	動態分配陣列記憶體(廢棄)
	 //	static Low_area_data = (typedef_Low_area_data*)malloc(sizeof(typedef_Low_area_data)*area_data_record);
	 //	讀入資料
	 //	宣告結構陣列
	 static struct Lowlevel_area_data lowlevel_area_data[area_data_record];
	 //	結構陣列項目初始值為-1，僅有name設定項能夠添加結構陣列項目量值
	 static int area_data_number = -1;
	 if(strcmp(name,config_item)==0){
	 //		設定結構陣列個數並且讀取name值
	 area_data_number = area_data_number + 1;
	 sprintf(lowlevel_area_data[area_data_number].name,"%s",config_item);

	 }else if(strcmp(initial_town,config_item)==0){
	 //		檢查name是否已初始化
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;
	 config_item = atoi(config_item);
	 sprintf(lowlevel_area_data[area_data_number].initial_town,"%d",config_item);

	 }else if(strcmp(area_type,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;
	 config_item = atoi(config_item);
	 sprintf(lowlevel_area_data[area_data_number].area_type,"%d",config_item);

	 }else if(strcmp(higher_area,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;



	 }else if(strcmp(subordinate_area,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;

	 }else if(strcmp(special_house,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;

	 }else if(strcmp(pokemon_list,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;

	 }else if(strcmp(pokemon_level,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;

	 }else if(strcmp(NPC_house,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;

	 }else if(strcmp(NPC_list,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;

	 }else if(strcmp(special_items,config_item)==0){
	 if(checkStructIsInitialized(this_system,area_data_number))return 1;

	 }else{

	 }
	 */
	return this_system_statue = &system_statue_ok;
}

char** skillDataSystem(char config_item[64],char config_variable[64],int call_type,
		char *callback_variable,char **incoming_struct,char source_system[60]){
	char this_system[] = "skillDataSystem";
	char logger_message[80];
	int severity_level = 0;

//	配置系統運行狀況報告
	char *system_statue_ok = "ok";
	char *system_statue_error = "error";
	char **this_system_statue = &system_statue_error;
	this_system_statue = &system_statue_ok;

//	獲取運行狀態
	char game_state[30];
	char *get_game_state = game_state;
	int eventManageSystem_type = 0;
	int call_record = operatingStateManageSystem(this_system, get_game_state,eventManageSystem_type);
	if(call_record == -1){
		severity_level = 3;
		sprintf(logger_message, "An error occurred while calling the Operating State Manage System");
		getLogger(severity_level, this_system, logger_message);
		return this_system_statue = &system_statue_error;
	}

//  計算讀取資料個數
	static int skill_data_record = 0;
	char number_of_reading[20] = "number of loadings";
//  checkStructIsInitialized所需要的變數數目
	int CSIItype = 5;
//	紀錄目前已讀取項目及讀取檔案設定進度
//	PRE_INITIALIZATION狀態之後請勿更動此數值
	static int now_struct_num = -1;
//  宣告結構指標
	static struct Lowlevel_skill_data *Lowlevel_skill_data_entity;

//	#PRE_INITIALIZATION
	if(strcmp(get_game_state, "PRE_INITIALIZATION") == 0){
//			運行狀態中被第一次呼叫
		if(call_record == 0){
			if(readConfiguration("skill_data_system") == -1){
				if(initializationErrorMessager(this_system, "skill_data_system") == -1) return this_system_statue = &system_statue_error;
			}else{
				return this_system_statue = &system_statue_ok;
			}
		}
		if(skill_data_record == 0){
			if(strcmp(config_item, number_of_reading) == 0 && atoi(config_variable) != 0){
				skill_data_record = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "There are %d settings that will be loaded",
						skill_data_record);
				getLogger(severity_level, this_system, logger_message);

//				初始化結構
				Lowlevel_skill_data_entity = (struct Lowlevel_skill_data*)malloc(
						sizeof(struct Lowlevel_skill_data) * skill_data_record);

//				初始化checkStructIsInitialized函式
				char initialization[] = "initialization";
				checkStructIsInitialized(this_system, initialization, CSIItype);
				return this_system_statue = &system_statue_ok;
			}else{
				severity_level = 3;
				sprintf(logger_message, "No number of reading is find");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}

		char name[] = "name";
		char classification_attribute_num[] = "classification_attribute_num";
		char classification_attribute[] = "classification_attribute";
		char attribute_num[] = "attribute_num";
		char attribute[] = "attribute";
		char skill_power[] = "skill_power";
		char accuracy[] = "accuracy";
		char chant_round[] = "chant_round";
		char call_base_event_system_num[] = "call_base_event_system_num";
		char call_base_event_system[] = "call_base_event_system";

//		開始輸入資料

		if(strcmp(config_item, name) == 0){
			now_struct_num++;
			sprintf(Lowlevel_skill_data_entity[now_struct_num].name, "%s", config_variable);

			severity_level = 1;
			sprintf(logger_message, "The name_variable is %s be loaded", Lowlevel_skill_data_entity[now_struct_num].name);
			getLogger(severity_level, this_system, logger_message);

//			結構初始化
			Lowlevel_skill_data_entity[now_struct_num].classification_attribute_num = -1;
			Lowlevel_skill_data_entity[now_struct_num].attribute_num = -1;
			Lowlevel_skill_data_entity[now_struct_num].accuracy = -1;
			Lowlevel_skill_data_entity[now_struct_num].chant_round = -1;
			Lowlevel_skill_data_entity[now_struct_num].classification_attribute_num = -1;

//			呼叫初始化函式
			severity_level = 1;
			sprintf(logger_message, "The Initialize system will be loading");
			getLogger(severity_level, this_system, logger_message);

			checkStructIsInitialized(this_system, name, CSIItype);
		}else if(strcmp(config_item, classification_attribute_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_skill_data_entity[now_struct_num].classification_attribute_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The classification_attribute_num %d is done",
						Lowlevel_skill_data_entity[now_struct_num].classification_attribute_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The classification_attribute_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, classification_attribute) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading classification attribute setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_skill_data_entity[now_struct_num].classification_attribute_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The classification_attribute_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading classification attribute setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_skill_data_entity[now_struct_num].classification_attribute = malloc(
						sizeof(char*)
								* Lowlevel_skill_data_entity[now_struct_num]
										.classification_attribute_num);
				for(int m = 0;
						m < Lowlevel_skill_data_entity[now_struct_num].classification_attribute_num;
						m++){
					Lowlevel_skill_data_entity[now_struct_num].classification_attribute[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The classification_attribute char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_skill_data_entity[now_struct_num].classification_attribute[0],
						"%s", config_variable);

				severity_level = 1;
				sprintf(logger_message, "The classification_attribute variable %s is done",
						Lowlevel_skill_data_entity[now_struct_num].classification_attribute[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_skill_data_entity[now_struct_num].classification_attribute[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The classification_attribute variable %s is done",
						Lowlevel_skill_data_entity[now_struct_num].classification_attribute[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, attribute_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_skill_data_entity[now_struct_num].attribute_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute_num %d is done",
						Lowlevel_skill_data_entity[now_struct_num].attribute_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The attribute_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, attribute) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading attribute setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_skill_data_entity[now_struct_num].attribute_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The attribute_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading attribute setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_skill_data_entity[now_struct_num].attribute = malloc(
						sizeof(char*) * Lowlevel_skill_data_entity[now_struct_num].attribute_num);
				for(int m = 0; m < Lowlevel_skill_data_entity[now_struct_num].attribute_num; m++){
					Lowlevel_skill_data_entity[now_struct_num].attribute[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The attribute char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_skill_data_entity[now_struct_num].attribute[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute variable %s is done",
						Lowlevel_skill_data_entity[now_struct_num].attribute[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_skill_data_entity[now_struct_num].attribute[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The attribute variable %s is done",
						Lowlevel_skill_data_entity[now_struct_num].attribute[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, skill_power) == 0){
//			double
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atof(config_variable) != 0){
				Lowlevel_skill_data_entity[now_struct_num].skill_power = atof(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The skill_power %lf is done",
						Lowlevel_skill_data_entity[now_struct_num].skill_power);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The skill_power value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, accuracy) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_skill_data_entity[now_struct_num].accuracy = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The accuracy %d is done",
						Lowlevel_skill_data_entity[now_struct_num].accuracy);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The accuracy value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, chant_round) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_skill_data_entity[now_struct_num].chant_round = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The chant_round %d is done",
						Lowlevel_skill_data_entity[now_struct_num].chant_round);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The chant_round value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, call_base_event_system_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_skill_data_entity[now_struct_num].call_base_event_system_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system_num %d is done",
						Lowlevel_skill_data_entity[now_struct_num].call_base_event_system_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The call_base_event_system_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, call_base_event_system) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading call base event system setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_skill_data_entity[now_struct_num].call_base_event_system_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The call_base_event_system_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading call base event system setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_skill_data_entity[now_struct_num].call_base_event_system = malloc(
						sizeof(char*)
								* Lowlevel_skill_data_entity[now_struct_num]
										.call_base_event_system_num);
				for(int m = 0;
						m < Lowlevel_skill_data_entity[now_struct_num].call_base_event_system_num;
						m++){
					Lowlevel_skill_data_entity[now_struct_num].call_base_event_system[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The call_base_event_system char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//			寫入資料
				sprintf(Lowlevel_skill_data_entity[now_struct_num].call_base_event_system[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system variable %s is done",
						Lowlevel_skill_data_entity[now_struct_num].call_base_event_system[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//			初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//			第一筆以後的資料
				sprintf(Lowlevel_skill_data_entity[now_struct_num].call_base_event_system[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system variable %s is done",
						Lowlevel_skill_data_entity[now_struct_num].call_base_event_system[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else{
			severity_level = 2;
			sprintf(logger_message, "No variables found to meet the requirements");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "Please check if there are variable errors ");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "or annotations set the wrong situation");
			getLogger(severity_level, this_system, logger_message);
		}
	}else if(strcmp(get_game_state, "INITIALIZATION") == 0){
//		讀取
		if(call_type == 0){
			int find_name_record = 0;
			int find_name_num;
			for(int m = 0; m <= now_struct_num; m++){
				if(strcmp(Lowlevel_skill_data_entity[m].name, config_item) == 0){
					find_name_record++;
					find_name_num = m;
					break;
				}
			}
			if(find_name_record == 0){
				severity_level = 2;
				sprintf(logger_message, "The Name variable %s can not be found", config_item);
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}else if(find_name_record == 1){
				if(strcmp("classification_attribute_num", config_variable) == 0
						|| strcmp("attribute_num", config_variable) == 0
						|| strcmp("accuracy", config_variable) == 0
						|| strcmp("chant_round", config_variable) == 0
						|| strcmp("call_base_event_system_num", config_variable) == 0){
//					int
					if(strcmp("classification_attribute_num", config_variable) == 0) sprintf(
							callback_variable, "int.%d",
							Lowlevel_skill_data_entity[find_name_num].classification_attribute_num);
					if(strcmp("attribute_num", config_variable) == 0) sprintf(callback_variable,
							"int.%d", Lowlevel_skill_data_entity[find_name_num].attribute_num);
					if(strcmp("accuracy", config_variable) == 0) sprintf(callback_variable,
							"int.%d", Lowlevel_skill_data_entity[find_name_num].accuracy);
					if(strcmp("chant_round", config_variable) == 0) sprintf(callback_variable,
							"int.%d", Lowlevel_skill_data_entity[find_name_num].chant_round);
					if(strcmp("call_base_event_system_num", config_variable) == 0) sprintf(
							callback_variable, "int.%d",
							Lowlevel_skill_data_entity[find_name_num].call_base_event_system_num);

					severity_level = 1;
					sprintf(logger_message, "The name:%s variables:%s is be read %s for %s", config_item,
							config_variable, callback_variable, source_system);
					getLogger(severity_level, this_system, logger_message);
				}else if(strcmp("classification_attribute", config_variable) == 0
						|| strcmp("attribute", config_variable) == 0
						|| strcmp("call_base_event_system", config_variable) == 0){
//					**char
					if(strcmp("classification_attribute", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_skill_data_entity[find_name_num].classification_attribute;
					}else if(strcmp("attribute", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_skill_data_entity[find_name_num].attribute;
					}else if(strcmp("call_base_event_system", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_skill_data_entity[find_name_num].call_base_event_system;
					}
				}
			}
		}
	}
	return this_system_statue = &system_statue_ok;
}

char** pokemonDataSystem(char config_item[64],char config_variable[64],int call_type,
		char *callback_variable,char **incoming_struct,char source_system[60]){
	char this_system[] = "pokemonDataSystem";
	char logger_message[80];
	int severity_level = 0;

//	配置系統運行狀況報告
	char *system_statue_ok = "ok";
	char *system_statue_error = "error";
	char **this_system_statue = &system_statue_error;
	this_system_statue = &system_statue_error;

//	獲取運行狀態
	char game_state[30];
	char *get_game_state = game_state;
	int eventManageSystem_type = 0;
	int call_record = operatingStateManageSystem(this_system, get_game_state,eventManageSystem_type);
	if(call_record == -1){
		severity_level = 3;
		sprintf(logger_message, "An error occurred while calling the Operating State Manage System");
		getLogger(severity_level, this_system, logger_message);
		return this_system_statue = &system_statue_error;
	}

//  計算讀取資料個數
	static int pokemon_data_record = 0;
	char number_of_reading[20] = "number of loadings";
//     checkStructIsInitialized所需要的變數數目
	int CSIItype = 5;
//  宣告結構指標
	static struct Lowlevel_pokemon_data *Lowlevel_pokemon_data_entity;
//	紀錄目前已讀取項目及讀取檔案設定進度
//	PRE_INITIALIZATION狀態之後請勿更動此數值
	static int now_struct_num = -1;
//	記錄所有name資料
	static char** name_list;

//	#PRE_INITIALIZATION
	if(strcmp(get_game_state, "PRE_INITIALIZATION") == 0){
//		運行狀態中被第一次呼叫
//		##警告，這邊為了Final作業改了呼叫結構
		char file_name[60];
		if(call_record == 0){
			sprintf(file_name,"pokemon_data_system.%s",source_system);
			if(readConfiguration(file_name) == -1){
				if(initializationErrorMessager(this_system, "pokemon_data_system") == -1)
					return this_system_statue = &system_statue_error;
			}else{
				return this_system_statue = &system_statue_ok;
			}
		}
//   	 初始化結構
		if(pokemon_data_record == 0){
			if(strcmp(config_item, number_of_reading) == 0 && atoi(config_variable) != 0){
				pokemon_data_record = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "There are %d settings that will be loaded",pokemon_data_record);
				getLogger(severity_level, this_system, logger_message);

				Lowlevel_pokemon_data_entity = (struct Lowlevel_pokemon_data*)malloc(
						sizeof(struct Lowlevel_pokemon_data) * pokemon_data_record);

//				初始化checkStructIsInitialized函式
				char initialization[] = "initialization";
				checkStructIsInitialized(this_system, initialization, CSIItype);
				return this_system_statue = &system_statue_ok;
			}else{
				severity_level = 3;
				sprintf(logger_message, "No number of reading is find");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}

		char name[] = "name";
		char grading_curve_type[] = "grading_curve_type";
		char attributes_num[] = "attributes_num";
		char attributes_type[] = "attributes_type";
		char base_ability_value[] = "base_ability_value";
		char ability_value_add_table[] = "ability_value_add_table";
		char racial_skills_table_num[] = "racial_skills_table_num";
		char racial_skills_table[] = "racial_skills_table";
		char racial_skills_level[] = "racial_skills_level";
		char skills_table_num[] = "skills_table_num";
		char skills_table[] = "skills_table";

//		開始輸入資料

		if(strcmp(config_item, name) == 0){
			now_struct_num++;
			sprintf(Lowlevel_pokemon_data_entity[now_struct_num].name, "%s", config_variable);

			severity_level = 1;
			sprintf(logger_message, "The name_variable is %s be loaded", Lowlevel_pokemon_data_entity[now_struct_num].name);
			getLogger(severity_level, this_system, logger_message);

//			結構初始化
			Lowlevel_pokemon_data_entity[now_struct_num].grading_curve_type = -1;
			Lowlevel_pokemon_data_entity[now_struct_num].attributes_num = -1;
			Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table_num = -1;
			Lowlevel_pokemon_data_entity[now_struct_num].skills_table_num = -1;

//			呼叫初始化函式
			severity_level = 1;
			sprintf(logger_message, "The Initialize system will be loading");
			getLogger(severity_level, this_system, logger_message);

			checkStructIsInitialized(this_system, name, CSIItype);
		}else if(strcmp(config_item, grading_curve_type) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_pokemon_data_entity[now_struct_num].grading_curve_type = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The grading_curve_type %d is done",
						Lowlevel_pokemon_data_entity[now_struct_num].grading_curve_type);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The grading_curve_type value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, attributes_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_pokemon_data_entity[now_struct_num].attributes_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attributes_num %d is done",
						Lowlevel_pokemon_data_entity[now_struct_num].attributes_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The attributes_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, attributes_type) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_pokemon_data_entity[now_struct_num].attributes_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The attributes_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading attributes type setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_pokemon_data_entity[now_struct_num].attributes_type = malloc(
						sizeof(char*)
								* Lowlevel_pokemon_data_entity[now_struct_num].attributes_num);
				for(int m = 0; m < Lowlevel_pokemon_data_entity[now_struct_num].attributes_num;
						m++){
					Lowlevel_pokemon_data_entity[now_struct_num].attributes_type[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The attributes_type char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_pokemon_data_entity[now_struct_num].attributes_type[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attributes_type variable %s is done",
						Lowlevel_pokemon_data_entity[now_struct_num].attributes_type[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_pokemon_data_entity[now_struct_num].attributes_type[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The attributes_type variable %s is done",
						Lowlevel_pokemon_data_entity[now_struct_num].attributes_type[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, base_ability_value) == 0){
//			struct struct Lowlevel_ability_value
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

			char *in_base_ability_value;
			char space[] = " ";

			char HP[] = "HP";
			char attack[] = "attack";
			char defense[] = "defense";
			char mana_attack[] = "mana_attack";
			char mana_defense[] = "mana_defense";
			char evasion_rate[] = "evasion_rate";
			char speed[] = "speed";
			char luck[] = "luck";

			strtok(config_variable, space);
			in_base_ability_value = strtok(NULL, space);

			if(strcmp(config_variable, HP) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.HP = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_HP %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.HP);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_HP value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, attack) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.attack = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_attack %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.attack);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_attack value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, defense) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.defense = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_defense %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.defense);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_defense value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, mana_attack) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.mana_attack = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_mana_attack %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.mana_attack);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_mana_attack value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, mana_defense) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.mana_defense = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_mana_defense %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.mana_defense);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_mana_defense value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, evasion_rate) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.evasion_rate = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_evasion_rate %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.evasion_rate);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_evasion_rate value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, speed) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.speed = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_speed %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.speed);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_speed value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, luck) == 0){
				if(atoi(in_base_ability_value) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.luck = atoi(
							in_base_ability_value);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_luck %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].base_ability_value.luck);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_luck value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}
		}else if(strcmp(config_item, ability_value_add_table) == 0){
//			struct struct Lowlevel_ability_value
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

			char *in_ability_add_table;
			char space[] = " ";

			char HP[] = "HP";
			char attack[] = "attack";
			char defense[] = "defense";
			char mana_attack[] = "mana_attack";
			char mana_defense[] = "mana_defense";
			char evasion_rate[] = "evasion_rate";
			char speed[] = "speed";
			char luck[] = "luck";

			strtok(config_variable, space);
			in_ability_add_table = strtok(NULL, space);

			if(strcmp(config_variable, HP) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.HP = atoi(
							in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_HP %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.HP);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_HP value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, attack) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.attack = atoi(
							in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_attack %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.attack);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_attack value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, defense) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.defense = atoi(
							in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_defense %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.defense);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_defense value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, mana_attack) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.mana_attack = atoi(
							in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_mana_attack %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.mana_attack);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_mana_attack value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, mana_defense) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table
							.mana_defense = atoi(in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_mana_defense %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table
														.mana_defense);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_mana_defense value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, evasion_rate) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table
							.evasion_rate = atoi(in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_evasion_rate %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.evasion_rate);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_evasion_rate value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, speed) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.speed = atoi(
							in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_speed %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.speed);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_speed value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}else if(strcmp(config_variable, luck) == 0){
				if(atoi(in_ability_add_table) != 0){
					Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.luck = atoi(
							in_ability_add_table);

					severity_level = 1;
					sprintf(logger_message, "The base_ability_luck %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].ability_value_add_table.luck);
					getLogger(severity_level, this_system, logger_message);
				}else{
					severity_level = 3;
					sprintf(logger_message, "The base_ability_luck value is warn");
					getLogger(severity_level, this_system, logger_message);
					return this_system_statue = &system_statue_error;
				}
			}
		}else if(strcmp(config_item, racial_skills_table_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The racial_skills_table_num %d is done",
						Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The racial_skills_table_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, racial_skills_table) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The racial_skills_table_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading racial skills table setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//		第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table = malloc(
						sizeof(char*)
								* Lowlevel_pokemon_data_entity[now_struct_num]
										.racial_skills_table_num);
				for(int m = 0;
						m < Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table_num;
						m++){
					Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The racial_skills_table char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The racial_skills_table variable %s is done",
						Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The racial_skills_table variable %s is done",
						Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, racial_skills_level) == 0){
//			int*
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_table_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The racial_skills_table_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading racial skills table setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			準備輸入
			if(atoi(config_variable) != 0){
				int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
				if(CSIInum == 0){
//					初始化
					Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_level = (int *)malloc(
							sizeof(int)
									* Lowlevel_pokemon_data_entity[now_struct_num]
											.racial_skills_table_num);
					severity_level = 1;
					sprintf(logger_message,
							"The racial_skills_level char array memory space allocated");
					getLogger(severity_level, this_system, logger_message);

//				第一筆資料
					Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_level[0] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The racial_skills_level variable %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_level[0]);
					getLogger(severity_level, this_system, logger_message);
				}else if(CSIInum == -1){
//				初始化錯誤
					severity_level = 3;
					sprintf(logger_message, "The checkStructIsInitialized system is warn");
					getLogger(severity_level, this_system, logger_message);
				}else{
					Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_level[CSIInum] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The racial_skills_level variable %d is done",
							Lowlevel_pokemon_data_entity[now_struct_num].racial_skills_level[CSIInum]);
					getLogger(severity_level, this_system, logger_message);
				}
			}else{
				severity_level = 3;
				sprintf(logger_message, "The racial_skills_table_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, skills_table_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_pokemon_data_entity[now_struct_num].skills_table_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The skills_table_num variable %d is done",
						Lowlevel_pokemon_data_entity[now_struct_num].skills_table_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The skills_table_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, skills_table) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_pokemon_data_entity[now_struct_num].skills_table_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The skills_table_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading skills table setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_pokemon_data_entity[now_struct_num].skills_table = malloc(
						sizeof(char*)
								* Lowlevel_pokemon_data_entity[now_struct_num].skills_table_num);
				for(int m = 0; m < Lowlevel_pokemon_data_entity[now_struct_num].skills_table_num;
						m++){
					Lowlevel_pokemon_data_entity[now_struct_num].skills_table[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The skills_table char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_pokemon_data_entity[now_struct_num].skills_table[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The skills_table variable %s is done",
						Lowlevel_pokemon_data_entity[now_struct_num].skills_table[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_pokemon_data_entity[now_struct_num].skills_table[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The subordinate_area variable %s is done",
						Lowlevel_pokemon_data_entity[now_struct_num].skills_table[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else{
			severity_level = 2;
			sprintf(logger_message, "No variables found to meet the requirements");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "Please check if there are variable errors ");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "or annotations set the wrong situation");
			getLogger(severity_level, this_system, logger_message);
		}
	}else if(strcmp(get_game_state, "INITIALIZATION") == 0){
//		給予name列表
		if(call_type == 2){
			static int call_num_1 = 0;
			if(call_num_1 == 0){
				call_num_1++;
				sprintf(callback_variable,"%d", now_struct_num);
			}else if(call_num_1 == 1){
				call_num_1 = 0;
				return name_list;
			}
//		讀取
		}else if(call_type == 0){
			int find_name_record = 0;
			int find_name_num;
			for(int m = 0; m <= now_struct_num; m++){
				if(strcmp(Lowlevel_pokemon_data_entity[m].name, config_item) == 0){
					find_name_record++;
					find_name_num = m;
					break;
				}
			}
			if(find_name_record == 0){
				severity_level = 2;
				sprintf(logger_message, "The Name variable %s can not be found", config_item);
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}else if(find_name_record == 1){
				if(strcmp("grading_curve_type", config_variable) == 0
						|| strcmp("attributes_num", config_variable) == 0
						|| strcmp("racial_skills_table_num", config_variable) == 0
						|| strcmp("skills_table_num", config_variable) == 0){
//					int
//					##int.記得改回來
					if(strcmp("grading_curve_type", config_variable) == 0) sprintf(
							callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].grading_curve_type);
					if(strcmp("attributes_num", config_variable) == 0) sprintf(callback_variable,
							"int.%d", Lowlevel_pokemon_data_entity[find_name_num].attributes_num);
					if(strcmp("racial_skills_table_num", config_variable) == 0) sprintf(
							callback_variable, "int.%d",
							Lowlevel_pokemon_data_entity[find_name_num].racial_skills_table_num);

					severity_level = 1;
					sprintf(logger_message, "The name:%s variables:%s is be read %s for %s", config_item,
							config_variable, callback_variable, source_system);
					getLogger(severity_level, this_system, logger_message);
				}else if(strcmp("attributes_type", config_variable) == 0
						|| strcmp("racial_skills_table", config_variable) == 0
						|| strcmp("skills_table", config_variable) == 0){
//					**char
					if(strcmp("attributes_type", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_pokemon_data_entity[find_name_num].attributes_type;
					}else if(strcmp("racial_skills_table", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_pokemon_data_entity[find_name_num].racial_skills_table;
					}else if(strcmp("skills_table", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_pokemon_data_entity[find_name_num].skills_table;
					}
				}else if(strcmp("B.HP", config_variable) == 0
						|| strcmp("B.attack", config_variable) == 0
						|| strcmp("B.defense", config_variable) == 0
						|| strcmp("B.mana_attack", config_variable) == 0
						|| strcmp("B.mana_defense", config_variable) == 0
						|| strcmp("B.evasion_rate", config_variable) == 0
						|| strcmp("B.speed", config_variable) == 0
						|| strcmp("B.luck", config_variable) == 0){
//					int
//					##int.記得改回來
					if(strcmp("B.HP", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.HP);
					if(strcmp("B.attack", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.attack);
					if(strcmp("B.defense", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.defense);
					if(strcmp("B.mana_attack", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.mana_attack);
					if(strcmp("B.mana_defence", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.mana_defense);
					if(strcmp("B.evasion_rate", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.evasion_rate);
					if(strcmp("B.defense", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.speed);
					if(strcmp("B.luck", config_variable) == 0) sprintf(callback_variable, "%d",
							Lowlevel_pokemon_data_entity[find_name_num].base_ability_value.luck);

					severity_level = 1;
					sprintf(logger_message, "The name:%s variables:%s is be read %s for %s",
							config_item, config_variable, callback_variable, source_system);



				}
			}
		}else if(call_type == 3){
//			進行資料初始化
//			編篡name陣列
			static int call_num = 0;
			if(call_num == 1){
//				初始化只能進行一次
				severity_level = 3;
				sprintf(logger_message, "Can only be initialized once!");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			name_list = malloc(sizeof(char*)*now_struct_num);
			for(int m = 0;m <= now_struct_num;m++)name_list[m] = malloc(sizeof(char) * 64);
			for(int m = 0;m <= now_struct_num;m++){
				sprintf(name_list[m],"%s",Lowlevel_pokemon_data_entity[m].name);
				severity_level = 1;
				sprintf(logger_message, "Name list var %s be ready",name_list[m]);
				getLogger(severity_level, this_system, logger_message);
			}



			call_num++;
		}
	}
	return this_system_statue = &system_statue_ok;
}
char** propsDataSystem(char config_item[64],char config_variable[64],int call_type,char *callback_variable,
		char **incoming_struct,char source_system[60]){
	char this_system[] = "propsDataSystem";
	char logger_message[80];
	int severity_level = 0;

//	配置系統運行狀況報告
	char *system_statue_ok = "ok";
	char *system_statue_error = "error";
	char **this_system_statue = &system_statue_error;
	this_system_statue = &system_statue_error;

//	獲取運行狀態
	char game_state[30];
	char *get_game_state = game_state;
	int eventManageSystem_type = 0;
	int call_record = operatingStateManageSystem(this_system, get_game_state,eventManageSystem_type);
	if(call_record == -1){
		severity_level = 3;
		sprintf(logger_message, "An error occurred while calling the Operating State Manage System");
		getLogger(severity_level, this_system, logger_message);
		return this_system_statue = &system_statue_error;
	}

//  計算讀取資料個數
	static int props_data_record = 0;
	char number_of_reading[20] = "number of loadings";
//  checkStructIsInitialized所需要的變數數目
	int CSIItype = 5;
//  宣告結構指標
	static struct Lowlevel_props_data *Lowlevel_props_data_entity;
//	紀錄目前已讀取項目及讀取檔案設定進度
//	PRE_INITIALIZATION狀態之後請勿更動此數值
	static int now_struct_num = -1;

//	#PRE_INITIALIZATION
	if(strcmp(get_game_state, "PRE_INITIALIZATION") == 0){
//		運行狀態中被第一次呼叫
		if(call_record == 0){
			if(readConfiguration("props_data_system") == -1){
				if(initializationErrorMessager(this_system, "props_data_system") == -1) return this_system_statue = &system_statue_error;
			}else{
				return this_system_statue = &system_statue_ok;
			}
		}
//   	 初始化結構
		if(props_data_record == 0){
			if(strcmp(config_item, number_of_reading) == 0 && atoi(config_variable) != 0){
				props_data_record = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "There are %d settings that will be loaded",
						props_data_record);
				getLogger(severity_level, this_system, logger_message);

				Lowlevel_props_data_entity = (struct Lowlevel_props_data*)malloc(
						sizeof(struct Lowlevel_props_data) * props_data_record);

//				初始化checkStructIsInitialized函式
				char initialization[] = "initialization";
				checkStructIsInitialized(this_system, initialization, CSIItype);
				return this_system_statue = &system_statue_ok;
			}else{
				severity_level = 3;
				sprintf(logger_message, "No number of reading is find");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}

		char name[] = "name";
		char disposable_props[] = "disposable_props";
		char attribute_num[] = "attribute_num";
		char attribute[] = "attribute";
		char call_base_event_system_num[] = "call_base_event_system_num";
		char call_base_event_system[] = "call_base_event_system";

//		開始輸入資料

		if(strcmp(config_item, name) == 0){
			now_struct_num++;
			sprintf(Lowlevel_props_data_entity[now_struct_num].name, "%s", config_variable);

			severity_level = 1;
			sprintf(logger_message, "The name_variable is %s be loaded",
					Lowlevel_props_data_entity[now_struct_num].name);
			getLogger(severity_level, this_system, logger_message);

//			結構初始化
			Lowlevel_props_data_entity[now_struct_num].attribute_num = -1;
			Lowlevel_props_data_entity[now_struct_num].call_base_event_system_num = -1;

//			呼叫初始化函式
			severity_level = 1;
			sprintf(logger_message, "The Initialize system will be loading");
			getLogger(severity_level, this_system, logger_message);

			checkStructIsInitialized(this_system, name, CSIItype);

		}else if(strcmp(config_item, disposable_props) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_props_data_entity[now_struct_num].disposable_props = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The disposable_props variable %d is done",
						Lowlevel_props_data_entity[now_struct_num].disposable_props);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The disposable_props value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, attribute_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_props_data_entity[now_struct_num].attribute_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute_num variable %d is done",
						Lowlevel_props_data_entity[now_struct_num].attribute_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The disposable_props value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, attribute) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_props_data_entity[now_struct_num].attribute_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The attribute_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading attribute setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_props_data_entity[now_struct_num].attribute = malloc(
						sizeof(char*) * Lowlevel_props_data_entity[now_struct_num].attribute_num);
				for(int m = 0; m < Lowlevel_props_data_entity[now_struct_num].attribute_num; m++){
					Lowlevel_props_data_entity[now_struct_num].attribute[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The attribute char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_props_data_entity[now_struct_num].attribute[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute variable %s is done",
						Lowlevel_props_data_entity[now_struct_num].attribute[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_props_data_entity[now_struct_num].attribute[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The attribute variable %s is done",
						Lowlevel_props_data_entity[now_struct_num].attribute[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, call_base_event_system_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_props_data_entity[now_struct_num].call_base_event_system_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system_num variable %d is done",
						Lowlevel_props_data_entity[now_struct_num].call_base_event_system_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The call_base_event_system_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, call_base_event_system) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_props_data_entity[now_struct_num].call_base_event_system_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The call_base_event_system_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading call base event system setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_props_data_entity[now_struct_num].call_base_event_system = malloc(
						sizeof(char*)
								* Lowlevel_props_data_entity[now_struct_num]
										.call_base_event_system_num);
				for(int m = 0;
						m < Lowlevel_props_data_entity[now_struct_num].call_base_event_system_num;
						m++){
					Lowlevel_props_data_entity[now_struct_num].call_base_event_system[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,"The call_base_event_system char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_props_data_entity[now_struct_num].call_base_event_system[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system variable %s is done",
						Lowlevel_props_data_entity[now_struct_num].call_base_event_system[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_props_data_entity[now_struct_num].call_base_event_system[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system variable %s is done",
						Lowlevel_props_data_entity[now_struct_num].call_base_event_system[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else{
			severity_level = 2;
			sprintf(logger_message, "No variables found to meet the requirements");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "Please check if there are variable errors ");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "or annotations set the wrong situation");
			getLogger(severity_level, this_system, logger_message);
		}
	}else if(strcmp(get_game_state, "INITIALIZATION") == 0){
//		讀取
		if(call_type == 0){
			int find_name_record = 0;
			int find_name_num;
			for(int m = 0; m < now_struct_num; m++){
				if(strcmp(Lowlevel_props_data_entity[m].name, config_item) == 0){
					find_name_record++;
					find_name_num = m;
					break;
				}
			}
			if(find_name_record == 0){
				severity_level = 2;
				sprintf(logger_message, "The Name variable %s can not be found", config_item);
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}else if(find_name_record == 1){
				if(strcmp("attribute_num", config_variable) == 0
						|| strcmp("disposable_props", config_variable) == 0
						|| strcmp("call_base_event_system_num", config_variable) == 0){
//					int
					if(strcmp("attribute_num", config_variable) == 0) sprintf(callback_variable,
							"int.%d", Lowlevel_props_data_entity[find_name_num].attribute_num);
					if(strcmp("disposable_props", config_variable) == 0) sprintf(callback_variable,
							"int.%d", Lowlevel_props_data_entity[find_name_num].disposable_props);
					if(strcmp("call_base_event_system_num", config_variable) == 0) sprintf(
							callback_variable, "int.%d",
							Lowlevel_props_data_entity[find_name_num].call_base_event_system_num);

					severity_level = 1;
					sprintf(logger_message, "The name:%s variables:%s is be read %s for %s", config_item,
							config_variable, callback_variable, source_system);
					getLogger(severity_level, this_system, logger_message);
				}else if(strcmp("attribute", config_variable) == 0
						|| strcmp("call_base_event_system", config_variable) == 0){
//					**char
					if(strcmp("attribute", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_props_data_entity[find_name_num].attribute;
					}else if(strcmp("call_base_event_system", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_props_data_entity[find_name_num].call_base_event_system;
					}

					severity_level = 1;
					sprintf(logger_message, "The name:%s char**_variables:%s is be read",
							config_item, config_variable);
				}
			}
		}
	}
	return this_system_statue = &system_statue_ok;
}

char** npcDataSystem(char config_item[64],char config_variable[64],int call_type,char *callback_variable,
		char **incoming_struct,char source_system[60]){
	char this_system[] = "npcDataSystem";
	char logger_message[80];
	int severity_level = 0;

//	配置系統運行狀況報告
	char *system_statue_ok = "ok";
	char *system_statue_error = "error";
	char **this_system_statue = &system_statue_error;
	this_system_statue = &system_statue_error;

//	獲取運行狀態
	char game_state[30];
	char *get_game_state = game_state;
	int eventManageSystem_type = 0;
	int call_record = operatingStateManageSystem(this_system, get_game_state,eventManageSystem_type);
	if(call_record == -1){
		severity_level = 3;
		sprintf(logger_message, "An error occurred while calling the Operating State Manage System");
		getLogger(severity_level, this_system, logger_message);
		return this_system_statue = &system_statue_error;
	}

//  計算讀取資料個數
	static int NPC_data_record = 0;
	char number_of_reading[20] = "number of loadings";
//  checkStructIsInitialized所需要的變數數目
	int type = 5;
//  宣告結構指標
	static struct Lowlevel_NPC_data *Lowlevel_NPC_data_entity;
//	紀錄目前已讀取項目及讀取檔案設定進度
//	PRE_INITIALIZATION狀態之後請勿更動此數值
	static int now_struct_num = -1;

//	#PRE_INITIALIZATION
	if(strcmp(get_game_state, "PRE_INITIALIZATION") == 0){
//		運行狀態中被第一次呼叫
		if(call_record == 0){
			if(readConfiguration("npc_data_system") == -1){
				if(initializationErrorMessager(this_system, "npc_data_system") == -1) return this_system_statue = &system_statue_error;
			}else{
				return this_system_statue = &system_statue_ok;
			}
		}
//   	 初始化結構
		if(NPC_data_record == 0){
			if(strcmp(config_item, number_of_reading) == 0 && atoi(config_variable) != 0){
				NPC_data_record = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "There are %d settings that will be loaded",NPC_data_record);
				getLogger(severity_level, this_system, logger_message);

				Lowlevel_NPC_data_entity = (struct Lowlevel_NPC_data*)malloc(
						sizeof(struct Lowlevel_NPC_data) * NPC_data_record);

//				初始化checkStructIsInitialized函式
				char initialization[] = "initialization";
				checkStructIsInitialized(this_system, initialization, type);
				return this_system_statue = &system_statue_ok;
			}else{
				severity_level = 3;
				sprintf(logger_message, "No number of reading is find");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}

		char name[] = "name";
		char attribute_num[] = "attribute_num";
		char attribute[] = "attribute";
		char call_base_event_system_num[] = "call_base_event_system_num";
		char call_base_event_system[] = "call_base_event_system";

//		開始輸入資料

		if(strcmp(config_item, name) == 0){
			now_struct_num++;
			sprintf(Lowlevel_NPC_data_entity[now_struct_num].name, "%s", config_variable);

			severity_level = 1;
			sprintf(logger_message, "The name_variable is %s be loaded",
					Lowlevel_NPC_data_entity[now_struct_num].name);
			getLogger(severity_level, this_system, logger_message);

//			結構初始化
			Lowlevel_NPC_data_entity[now_struct_num].attribute_num = -1;
			Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system_num = -1;

//			呼叫初始化函式
			severity_level = 1;
			sprintf(logger_message, "The Initialize system will be loading");
			getLogger(severity_level, this_system, logger_message);

			checkStructIsInitialized(this_system, name, type);

		}else if(strcmp(config_item, attribute_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_NPC_data_entity[now_struct_num].attribute_num = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute_num variable %d is done",
						Lowlevel_NPC_data_entity[now_struct_num].attribute_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The disposable_props value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, attribute) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_NPC_data_entity[now_struct_num].attribute_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The attribute_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading attribute setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, type);
			if(CSIInum == 0){
				Lowlevel_NPC_data_entity[now_struct_num].attribute = malloc(
						sizeof(char*) * Lowlevel_NPC_data_entity[now_struct_num].attribute_num);
				for(int m = 0; m < Lowlevel_NPC_data_entity[now_struct_num].attribute_num; m++){
					Lowlevel_NPC_data_entity[now_struct_num].attribute[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The attribute char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_NPC_data_entity[now_struct_num].attribute[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The attribute variable %s is done",
						Lowlevel_NPC_data_entity[now_struct_num].attribute[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_NPC_data_entity[now_struct_num].attribute[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The attribute variable %s is done",
						Lowlevel_NPC_data_entity[now_struct_num].attribute[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, call_base_event_system_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system_num variable %d is done",
						Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The call_base_event_system_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, call_base_event_system) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading subordinate area setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The call_base_event_system_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading call base event system setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, type);
			if(CSIInum == 0){
				Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system = malloc(
						sizeof(char*)
								* Lowlevel_NPC_data_entity[now_struct_num]
										.call_base_event_system_num);
				for(int m = 0;
						m < Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system_num;
						m++){
					Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,"The call_base_event_system char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system variable %s is done",
						Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The call_base_event_system variable %s is done",
						Lowlevel_NPC_data_entity[now_struct_num].call_base_event_system[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else{
			severity_level = 2;
			sprintf(logger_message, "No variables found to meet the requirements");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "Please check if there are variable errors ");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "or annotations set the wrong situation");
			getLogger(severity_level, this_system, logger_message);
		}
	}else if(strcmp(get_game_state, "INITIALIZATION") == 0){
		//		讀取
		if(call_type == 0){
			int find_name_record = 0;
			int find_name_num;
			for(int m = 0; m <= now_struct_num; m++){
				if(strcmp(Lowlevel_NPC_data_entity[m].name, config_item) == 0){
					find_name_record++;
					find_name_num = m;
					break;
				}
			}
			if(find_name_record == 0){
				severity_level = 2;
				sprintf(logger_message, "The Name variable %s can not be found", config_item);
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}else if(find_name_record == 1){
				if(strcmp("attribute_num", config_variable) == 0
						|| strcmp("call_base_event_system_num", config_variable) == 0){
//					int
					if(strcmp("attribute_num", config_variable) == 0) sprintf(callback_variable,
							"int.%d", Lowlevel_NPC_data_entity[find_name_num].attribute_num);
					if(strcmp("call_base_event_system_num", config_variable) == 0) sprintf(
							callback_variable, "int.%d",
							Lowlevel_NPC_data_entity[find_name_num].call_base_event_system_num);

					severity_level = 1;
					sprintf(logger_message, "The name:%s variables:%s is be read %s for %s", config_item,
							config_variable, callback_variable, source_system);
					getLogger(severity_level, this_system, logger_message);
				}else if(strcmp("attribute", config_variable) == 0
						|| strcmp("call_base_event_system", config_variable) == 0){
//					**char
					if(strcmp("attribute", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_NPC_data_entity[find_name_num].attribute;
					}else if(strcmp("call_base_event_system", config_variable) == 0){
						severity_level = 1;
						sprintf(logger_message, "The name:%s char**_variables:%s is be return for %s",
								config_item, config_variable,source_system);
						getLogger(severity_level, this_system, logger_message);
						return Lowlevel_NPC_data_entity[find_name_num].call_base_event_system;
					}
					severity_level = 1;
					sprintf(logger_message, "The name:%s char**_variables:%s is be read",
							config_item, config_variable);
				}
			}
		}
	}
	return this_system_statue = &system_statue_ok;
}

char** triggerEventDataSystem(char config_item[64],char config_variable[64],int call_type,char *callback_variable,
		char **incoming_struct,char source_system[60]){
	char this_system[] = "triggerEventDataSystem";
	char logger_message[80];
	int severity_level = 0;

//	配置系統運行狀況報告
	char *system_statue_ok = "ok";
	char *system_statue_error = "error";
	char **this_system_statue = &system_statue_error;
	this_system_statue = &system_statue_error;

//	獲取運行狀態
	char game_state[30];
	char *get_game_state = game_state;
	int eventManageSystem_type = 0;
	int call_record = operatingStateManageSystem(this_system, get_game_state,eventManageSystem_type);
	if(call_record == -1){
		severity_level = 3;
		sprintf(logger_message, "An error occurred while calling the Operating State Manage System");
		getLogger(severity_level, this_system, logger_message);
		return this_system_statue = &system_statue_error;
	}

//  計算讀取資料個數
	static int trigger_event_data_record = 0;
	char number_of_reading[20] = "number of loadings";
//  checkStructIsInitialized所需要的變數數目
	int CSIItype = 5;
//  宣告結構指標
	static struct Lowlevel_trigger_event_data *Lowlevel_trigger_event_data_entity;

//	#PRE_INITIALIZATION
	if(strcmp(get_game_state, "PRE_INITIALIZATION") == 0){
//		運行狀態中被第一次呼叫
		if(call_record == 0){
			if(readConfiguration("trigger_event_data_system") == -1){
				if(initializationErrorMessager(this_system, "trigger_event_data_system") == -1) return this_system_statue = &system_statue_error;
			}else{
				return this_system_statue = &system_statue_ok;
			}
		}
//   	 初始化結構
		if(trigger_event_data_record == 0){
			if(strcmp(config_item, number_of_reading) == 0 && atoi(config_variable) != 0){
				trigger_event_data_record = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "There are %d settings that will be loaded",trigger_event_data_record);
				getLogger(severity_level, this_system, logger_message);

				Lowlevel_trigger_event_data_entity = (struct Lowlevel_trigger_event_data*)malloc(
						sizeof(struct Lowlevel_trigger_event_data) * trigger_event_data_record);

//				初始化checkStructIsInitialized函式
				char initialization[] = "initialization";
				checkStructIsInitialized(this_system, initialization, CSIItype);
				return this_system_statue = &system_statue_ok;
			}else{
				severity_level = 3;
				sprintf(logger_message, "No number of reading is find");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}

		char name[] = "name";
		char environment[] = "environment";
		char event_listener[] = "event_listener";
		char event_listener_conditions[] = "event_listener_conditions";

		char triggering_conditions_num[] = "triggering_conditions_num";
		char triggering_conditions[] = "triggering_conditions";
		char triggering_conditions_variableCA[] = "triggering_conditions_variableCA";
		char triggering_conditions_variableCB[] = "triggering_conditions_variableCB";
		char triggering_conditions_variableIA[] = "triggering_conditions_variableIA";
		char triggering_conditions_variableIB[] = "triggering_conditions_variableIB";

		char triggering_event_num[] = "triggering_event_num";
		char triggering_event[] = "triggering_event";
		char triggering_event_variableCA[] = "triggering_event_variableCA";
		char triggering_event_variableCB[] = "triggering_event_variableCB";
		char triggering_event_variableIA[] = "triggering_event_variableIA";
		char triggering_event_variableIB[] = "triggering_event_variableIB";

//		開始輸入資料
		static int now_struct_num = -1;

		if(strcmp(config_item, name) == 0){
			now_struct_num++;
			sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].name, "%s", config_variable);

			severity_level = 1;
			sprintf(logger_message, "The name_variable is %s be loaded",
					Lowlevel_trigger_event_data_entity[now_struct_num].name);
			getLogger(severity_level, this_system, logger_message);

//			結構初始化
			Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num = -1;
			Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num = -1;

//			呼叫初始化函式
			severity_level = 1;
			sprintf(logger_message, "The Initialize system will be loading");
			getLogger(severity_level, this_system, logger_message);

			checkStructIsInitialized(this_system, name, CSIItype);

		}else if(strcmp(config_item, environment) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].environment = atoi(config_variable);

				severity_level = 1;
				sprintf(logger_message, "The environment variable %d is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].environment);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The environment value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, event_listener) == 0){
//			char
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

			sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].event_listener,"%s",config_variable);
			severity_level = 1;
			sprintf(logger_message, "The event_listener variable %s is done",
					Lowlevel_trigger_event_data_entity[now_struct_num].event_listener);
			getLogger(severity_level, this_system, logger_message);
		}else if(strcmp(config_item, event_listener_conditions) == 0){
//			char
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

			sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].event_listener_conditions, "%s",
					config_variable);
			severity_level = 1;
			sprintf(logger_message, "The event_listener variable %s is done",
					Lowlevel_trigger_event_data_entity[now_struct_num].event_listener);
			getLogger(severity_level, this_system, logger_message);
		}else if(strcmp(config_item, triggering_conditions_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions_num variable %d is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, triggering_conditions) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions = malloc(
						sizeof(char*) * Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num);
				for(int m = 0; m < Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num; m++){
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions[0], "%s",
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions[CSIInum], "%s",
						config_variable);
				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, triggering_conditions_variableCA) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions variableCA setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCA = malloc(
						sizeof(char*)
								* Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_conditions_num);
				for(int m = 0;m < Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_conditions_num; m++){
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCA[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The triggering_conditions char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCA[0],
						"%s", config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions_variableCA variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCA[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCA[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions_variableCA variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCA[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, triggering_conditions_variableCB) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions variableCB setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCB = malloc(
						sizeof(char*)
								* Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_conditions_num);
				for(int m = 0;m < Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_conditions_num; m++){
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCB[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,"The triggering_conditions_variableCB char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCB[0],
						"%s", config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions_variableCB variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCB[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCB[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The triggering_conditions_variableCB variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableCB[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, triggering_conditions_variableIA) == 0){
//			int*
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions variableIA setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions variableIA setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			準備輸入
			if(atoi(config_variable) != 0){
				int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
				if(CSIInum == 0){
//					初始化
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIA = (int *)malloc(
							sizeof(int)
									* Lowlevel_trigger_event_data_entity[now_struct_num]
											.triggering_conditions_num);
					severity_level = 1;
					sprintf(logger_message,
							"The triggering_conditions_variableIA char array memory space allocated");
					getLogger(severity_level, this_system, logger_message);

//					第一筆資料
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIA[0] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The triggering_conditions_variableIA variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIA[0]);
					getLogger(severity_level, this_system, logger_message);
				}else if(CSIInum == -1){
//					初始化錯誤
					severity_level = 3;
					sprintf(logger_message, "The checkStructIsInitialized system is warn");
					getLogger(severity_level, this_system, logger_message);
				}else{
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIA[CSIInum] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The triggering_conditions_variableIA variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIA[CSIInum]);
					getLogger(severity_level, this_system, logger_message);
				}
			}else{
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_variableIA value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, triggering_conditions_variableIB) == 0){
//			int*
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions variableIB setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions variableIB setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			準備輸入
			if(atoi(config_variable) != 0){
				int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
				if(CSIInum == 0){
//					初始化
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIB = (int *)malloc(
							sizeof(int)
									* Lowlevel_trigger_event_data_entity[now_struct_num]
											.triggering_conditions_num);
					severity_level = 1;
					sprintf(logger_message,
							"The triggering_conditions_variableIB char array memory space allocated");
					getLogger(severity_level, this_system, logger_message);

//					第一筆資料
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIB[0] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The triggering_conditions_variableIB variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIB[0]);
					getLogger(severity_level, this_system, logger_message);
				}else if(CSIInum == -1){
//					初始化錯誤
					severity_level = 3;
					sprintf(logger_message, "The checkStructIsInitialized system is warn");
					getLogger(severity_level, this_system, logger_message);
				}else{
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIB[CSIInum] = atoi(
							config_variable);

					severity_level = 1;
					sprintf(logger_message, "The triggering_conditions_variableIB variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_variableIB[CSIInum]);
					getLogger(severity_level, this_system, logger_message);
				}
			}else{
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_variableIB value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, triggering_event_num) == 0){
//			int
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(atoi(config_variable) != 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num = atoi(
						config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_event_num variable %d is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num);
				getLogger(severity_level, this_system, logger_message);
			}else{
				severity_level = 3;
				sprintf(logger_message, "The triggering_event_num value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, triggering_event) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering event setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_event_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering event setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event = malloc(
						sizeof(char*)
								* Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_event_num);
				for(int m = 0;m < Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_event_num; m++){
					Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event[m] = malloc(
							sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The triggering_conditions char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event[0],
						"%s", config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_event variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event[CSIInum],
						"%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The triggering_event variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, triggering_event_variableCA) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message,
						"Failed to loading triggering event variableCA setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_event_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering conditions setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_variableCA = malloc(
						sizeof(char*)
								* Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num);
				for(int m = 0;m < Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num; m++){
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableCA[m] = malloc(sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The triggering_event_variableCA char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCA[0], "%s", config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_event_variableCA variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCA[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCA[CSIInum], "%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The triggering_event_variableCA variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCA[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, triggering_event_variableCB) == 0){
//			char**
//			偵錯
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message,"Failed to loading triggering event variableCB setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_event_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message, "Failed to loading triggering event variableCB setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			第一筆資料
			int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
			if(CSIInum == 0){
				Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_variableCB = malloc(
						sizeof(char*)
								* Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_event_num);
				for(int m = 0;m < Lowlevel_trigger_event_data_entity[now_struct_num]
										.triggering_event_num; m++){
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableCB[m] = malloc(sizeof(char) * 64);
				}

				severity_level = 1;
				sprintf(logger_message,
						"The triggering_conditions_variableCB char array memory space allocated");
				getLogger(severity_level, this_system, logger_message);

//				寫入資料
				sprintf(Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCB[0], "%s", config_variable);

				severity_level = 1;
				sprintf(logger_message, "The triggering_event_variableCB variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCB[0]);
				getLogger(severity_level, this_system, logger_message);
			}else if(CSIInum == -1){
//				初始化錯誤
				severity_level = 3;
				sprintf(logger_message, "The checkStructIsInitialized system is warn");
				getLogger(severity_level, this_system, logger_message);
			}else{
//				第一筆以後的資料
				sprintf(
						Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCB[CSIInum], "%s", config_variable);
				severity_level = 1;
				sprintf(logger_message, "The triggering_event_variableCB variable %s is done",
						Lowlevel_trigger_event_data_entity[now_struct_num]
								.triggering_event_variableCB[CSIInum]);
				getLogger(severity_level, this_system, logger_message);
			}
		}else if(strcmp(config_item, triggering_event_variableIA) == 0){
//			int*
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message,
						"Failed to loading triggering event variableIA setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_event_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_event_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message,
						"Failed to loading triggering event variableIA setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			準備輸入
			if(atoi(config_variable) != 0){
				int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
				if(CSIInum == 0){
//					初始化
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableIA = (int *)malloc(
							sizeof(int)
									* Lowlevel_trigger_event_data_entity[now_struct_num]
											.triggering_event_num);
					severity_level = 1;
					sprintf(logger_message,
							"The triggering_event_variableIA char array memory space allocated");
					getLogger(severity_level, this_system, logger_message);

//					第一筆資料
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableIA[0] = atoi(config_variable);

					severity_level = 1;
					sprintf(logger_message,
							"The triggering_event_variableIA variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num]
									.triggering_event_variableIA[0]);
					getLogger(severity_level, this_system, logger_message);
				}else if(CSIInum == -1){
//					初始化錯誤
					severity_level = 3;
					sprintf(logger_message, "The checkStructIsInitialized system is warn");
					getLogger(severity_level, this_system, logger_message);
				}else{
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableIA[CSIInum] = atoi(config_variable);

					severity_level = 1;
					sprintf(logger_message,
							"The triggering_event_variableIA variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num]
									.triggering_event_variableIA[CSIInum]);
					getLogger(severity_level, this_system, logger_message);
				}
			}else{
				severity_level = 3;
				sprintf(logger_message, "The triggering_event_variableIA value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else if(strcmp(config_item, triggering_event_variableIB) == 0){
//			int*
			if(now_struct_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The name_variable is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message,
						"Failed to loading triggering conditions variableIB setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
			if(Lowlevel_trigger_event_data_entity[now_struct_num].triggering_conditions_num == -1){
				severity_level = 3;
				sprintf(logger_message, "The triggering_conditions_num is not be loaded");
				getLogger(severity_level, this_system, logger_message);
				severity_level = 3;
				sprintf(logger_message,
						"Failed to loading triggering event variableIB setting");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}

//			準備輸入
			if(atoi(config_variable) != 0){
				int CSIInum = checkStructIsInitialized(this_system, config_item, CSIItype);
				if(CSIInum == 0){
//					初始化
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableIB = (int *)malloc(
							sizeof(int)
									* Lowlevel_trigger_event_data_entity[now_struct_num]
											.triggering_event_num);
					severity_level = 1;
					sprintf(logger_message,
							"The triggering_event_variableIB char array memory space allocated");
					getLogger(severity_level, this_system, logger_message);

//					第一筆資料
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableIB[0] = atoi(config_variable);

					severity_level = 1;
					sprintf(logger_message,
							"The triggering_event_variableIB variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num]
									.triggering_event_variableIB[0]);
					getLogger(severity_level, this_system, logger_message);
				}else if(CSIInum == -1){
//					初始化錯誤
					severity_level = 3;
					sprintf(logger_message, "The checkStructIsInitialized system is warn");
					getLogger(severity_level, this_system, logger_message);
				}else{
					Lowlevel_trigger_event_data_entity[now_struct_num]
							.triggering_event_variableIB[CSIInum] = atoi(config_variable);

					severity_level = 1;
					sprintf(logger_message,
							"The triggering_event_variableIB variable %d is done",
							Lowlevel_trigger_event_data_entity[now_struct_num]
									.triggering_event_variableIB[CSIInum]);
					getLogger(severity_level, this_system, logger_message);
				}
			}else{
				severity_level = 3;
				sprintf(logger_message, "The triggering_event_variableIB value is warn");
				getLogger(severity_level, this_system, logger_message);
				return this_system_statue = &system_statue_error;
			}
		}else{
			severity_level = 2;
			sprintf(logger_message, "No variables found to meet the requirements");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "Please check if there are variable errors ");
			getLogger(severity_level, this_system, logger_message);

			severity_level = 2;
			sprintf(logger_message, "or annotations set the wrong situation");
			getLogger(severity_level, this_system, logger_message);
		}
	}else if(strcmp(get_game_state, "INITIALIZATION") == 0){

	}
	return this_system_statue = &system_statue_ok;
}




int readConfiguration(char source_system[60]){
	char this_system[] = "readTheConfiguration";
	char logger_message[80];
	int severity_level;

	char single_config_info[64];
	char clone_config_info[64];
//	等號兩旁強制加空白
	char *equal_sign = "=";
	char *equal_linefeed = "\n";
	char *target_config_info;
	static int error_return = 0;
	int space_int = 0;
	char space_char;
	char *space_charstar = &space_char;
	char **space_charstar2 = &space_charstar;

	char area_data_system[] = "area_data_system";
	char pokemon_data_system[] = "pokemon_data_system";
	char props_data_system[] = "props_data_system";
	char npc_data_system[] = "npc_data_system";
	char skill_data_system[] = "skill_data_system";
	char trigger_event_data_system[] = "trigger_event_data_system";
	FILE *fp;

//	##警告：這裡的strtok是因應作業使用

	strtok(source_system,".");

	if(strcmp(source_system, area_data_system) == 0){
		fp = fopen("./area_data.txt", "r");
	}else if(strcmp(source_system, skill_data_system) == 0){
		fp = fopen("./skill_data.txt", "r");
	}else if(strcmp(source_system, pokemon_data_system) == 0){
//		##作業修改警告
		char *file_name = strtok(NULL, equal_sign);
		if(strcmp(file_name, "main") == 0){
			fp = fopen("./pokemon_data.txt", "r");
		}else{
			fp = fopen(file_name, "r");
		}
	}else if(strcmp(source_system, props_data_system) == 0){
		fp = fopen("./props_data.txt", "r");
	}else if(strcmp(source_system, npc_data_system) == 0){
		fp = fopen("./NPC_data.txt", "r");
	}else if(strcmp(source_system, trigger_event_data_system) == 0){
		fp = fopen("./trigger_event_data.txt", "r");
	}else{
		severity_level = 3;
		sprintf(logger_message, "No %s config is find", source_system);
		getLogger(severity_level, this_system, logger_message);
		return -1;
	}
	if(fp == NULL){ //判斷檔案是否為空
		severity_level = 3;
		sprintf(logger_message, "No %s config is find", source_system);
		getLogger(severity_level, this_system, logger_message);
		return -1;
	}
	while(!feof(fp)){ //feof判斷是否到檔案結尾，是的話返回true
//		開始讀取
		if(fgets(single_config_info, 64, fp) != NULL){ //fget，整行讀取，(讀入字串,最大讀入大小,目標檔案)
//			char *t_single_config_info = single_config_info;
//			killSpace(t_single_config_info);

			char annotation[] = "/";
			if(single_config_info[0] == annotation[0] && single_config_info[1] == annotation[0]) continue;

			strcpy(clone_config_info, single_config_info); //複製字串內容
			strtok(clone_config_info, equal_linefeed);
			strtok(clone_config_info, equal_sign); //比對刪除字串內容，遇到引數二則將其替代為/0
			target_config_info = strtok(NULL, equal_sign); //第二次呼叫NULL，則返回上次/0後內容

			if(target_config_info == NULL) continue;

//			debug
			severity_level = 1;
			sprintf(logger_message, "varible 1 = %s", clone_config_info);
			getLogger(severity_level, this_system, logger_message);

			severity_level = 1;
			sprintf(logger_message, "varible 2 = %s", target_config_info);
			getLogger(severity_level, this_system, logger_message);

//			傳輸配置
			if(strcmp(source_system, area_data_system) == 0){
				areaDataSystem(clone_config_info, target_config_info, space_int, "", space_charstar2,this_system);
			}else if(strcmp(source_system, skill_data_system) == 0){
				skillDataSystem(clone_config_info, target_config_info, space_int, "", space_charstar2,this_system);
			}else if(strcmp(source_system, pokemon_data_system) == 0){
				pokemonDataSystem(clone_config_info, target_config_info, space_int, "", space_charstar2,this_system);
			}else if(strcmp(source_system, props_data_system) == 0){
				propsDataSystem(clone_config_info, target_config_info, space_int, "", space_charstar2,this_system);
			}else if(strcmp(source_system, npc_data_system) == 0){
				npcDataSystem(clone_config_info, target_config_info, space_int, "", space_charstar2,this_system);
			}else if(strcmp(source_system, trigger_event_data_system) == 0){
				triggerEventDataSystem(clone_config_info, target_config_info, space_int, "", space_charstar2,this_system);
			}
		}
	}
	return error_return;
}



int eventManageSystem(char source_system[60]){
	return 0;
}

int main(int argc, char *argv[]){
//	system("mode con cols=120 lines=30");
	system("chcp 65001");

//	atexit(exitf);

	char this_system[] = "main";
	char logger_message[80];
	int severity_level = 0;

	int eventManageSystem_type;
	char game_state[30];
	char *get_game_state = game_state;
	int space_int = 0;
	char space_char;
	char *space_charstar = &space_char;
	char **space_charstar2 = &space_charstar;

//	非debug模式
//	getLogger(10, this_system, "");

	if(strcmp(argv[2],"non-debug") == 0)getLogger(10, this_system, "");

//	char error_return[10];
//
//	char *a1, *a2;
//	int type = 1;
//	char a3[] = "1";
//	eventManageSystem(a3, a2, type);

	sprintf(logger_message, "Starting Pokemon Version 0.1");
	getLogger(severity_level, this_system, logger_message);

	sprintf(logger_message, "Start to loading config");
	getLogger(severity_level, this_system, logger_message);

//	進入PRE_INITIALIZATION週期
	eventManageSystem_type = 1;
	operatingStateManageSystem(this_system, get_game_state, eventManageSystem_type);

	sprintf(logger_message, "Loading the config data...");
	getLogger(severity_level, this_system, logger_message);

	if(strcmp(argv[1],"homework") == 0){
		char **name_list;
		char name_num_char[10];
		char *name_num_char_star = name_num_char;
		int name_num;
		int now_pokemon_num;
		int have_pokemon_num = 0;
		char have_pokemon[30][30];
		int have_pokemon_num2[30];
		int have_pokemon_attributes = -1;
		int have_pokemon_HP[30];
		int rand_num;
		pokemonDataSystem("", "", space_int, "", space_charstar2,this_system);
//		grading_curve_type當作可否升級依據，1為否、2為是
//		強制進入INITIALIZATION週期
		operatingStateManageSystem(this_system, get_game_state, eventManageSystem_type);
//		獲取寶可夢標籤快取
		pokemonDataSystem("", "", 3, "", space_charstar2,this_system);
		pokemonDataSystem("", "", 2, name_num_char_star, space_charstar2,this_system);
		name_list = pokemonDataSystem("", "", 2, "", space_charstar2,this_system);
		name_num = atoi(name_num_char_star);
		if(name_num == 0){
			severity_level = 3;
			sprintf(logger_message, "There was an error reading the number of name");
			getLogger(severity_level, this_system, logger_message);
		}

		severity_level = 0;
		sprintf(logger_message,"wellcome to play pokemon");
		getLogger(severity_level, "main", logger_message);
//		sprintf(logger_message,"目前尚未針對手上沒有任何寶可夢不抓卻硬要選其他選項建立debug手段");
//		getLogger(severity_level, "main", logger_message);
//		sprintf(logger_message,"請先抓到至少一隻寶可夢再選擇其他選項");
//		getLogger(severity_level, "main", logger_message);

//		以時間為種子宣告亂數種子
		srand((unsigned)time(NULL));
		for(;1;){
			severity_level = 0;
			sprintf(logger_message,"請輸入選項: ");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(1). get a monster");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(2). list the monsters with max and min HP that you have catched");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(3). list the monsters that you have");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(4). check whether the monster can be evoluated, and evolute it");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(5). author info");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(6). exit");
			getLogger(severity_level, "main", logger_message);

			char input[5];
			gets(input);
			if(strcmp(input,"1") == 0){
				have_pokemon_num++;
//				生成亂數
				rand_num = rand() % (name_num + 1);

				if(have_pokemon_attributes == -1){
//					獲得第一隻寶可夢
					have_pokemon_attributes++;
					sprintf(have_pokemon[have_pokemon_attributes],"%s",name_list[rand_num]);
					have_pokemon_num2[have_pokemon_attributes] = 1;
					have_pokemon_HP[have_pokemon_attributes] = rand() % 101;
					now_pokemon_num = have_pokemon_attributes;
				}else{
					int find_record = -1;
					for(int m = 0;m <= have_pokemon_attributes;m++){
						if(strcmp(have_pokemon[m],name_list[rand_num]) == 0){
							find_record++;
							have_pokemon_num2[m]++;
							now_pokemon_num = m;
							break;
						}
					}
					if(find_record == -1){
						have_pokemon_attributes++;
						sprintf(have_pokemon[have_pokemon_attributes],"%s",name_list[rand_num]);
						have_pokemon_num2[have_pokemon_attributes] = 1;
						have_pokemon_HP[have_pokemon_attributes] = rand() % 101;
						now_pokemon_num = have_pokemon_attributes;
					}
				}


				severity_level = 0;
				sprintf(logger_message, "這是你的第%d隻神奇寶貝，名叫%s，HP是%d，請好好珍惜它", have_pokemon_num,
						have_pokemon[now_pokemon_num],have_pokemon_HP[now_pokemon_num]);
				getLogger(severity_level, "main", logger_message);
			}else if(strcmp(input,"2") == 0){
				int min = 0;
				int max = 0;

				for(int m = 0;m <= have_pokemon_attributes;m++){
					if(have_pokemon_HP[m] > have_pokemon_HP[max])max = m;
					if(have_pokemon_HP[m] < have_pokemon_HP[min])min = m;
				}

				severity_level = 0;
				sprintf(logger_message, "HP最大的神奇寶貝是%s，最小的則是%s",have_pokemon[max],have_pokemon[min]);
				getLogger(severity_level, "main", logger_message);

				severity_level = 0;
				sprintf(logger_message, "HP分別是最大的%d和最小的%d",have_pokemon_HP[max],have_pokemon_HP[min]);
				getLogger(severity_level, "main", logger_message);

			}else if(strcmp(input,"3") == 0){
				for(int m = 0;m <= have_pokemon_attributes;m++){
					severity_level = 0;
					sprintf(logger_message, "你擁有%d隻%s",have_pokemon_num2[m],have_pokemon[m]);
					getLogger(severity_level, "main", logger_message);
				}

			}else if(strcmp(input,"4") == 0){
				for(int m = 0;m <= have_pokemon_attributes;m++){
					int evoluate_num_int;
					char evoluate_num_char[10];
					char *evoluate_num_char_star = evoluate_num_char;

					char evoluate_name[60];
					char **evoluate_name_star2;

//					獲取升級數據
					pokemonDataSystem(have_pokemon[m], "grading_curve_type", 0,
							evoluate_num_char_star, space_charstar2, "main");

					if(atoi(evoluate_num_char) == 0){
						severity_level = 3;
						sprintf(logger_message, "尚未初始化升級數據");
						getLogger(severity_level, "main", logger_message);
					}else if(atoi(evoluate_num_char) == -1){
						severity_level = 0;
						sprintf(logger_message, "%s這隻寶可夢已經是最高型態，無法進化",have_pokemon[m]);
						getLogger(severity_level, "main", logger_message);
						continue;
					}else{
						evoluate_num_int = atoi(evoluate_num_char);
					}

//					比較數量與進行升級
					if(have_pokemon_num2[m] >= evoluate_num_int){
						have_pokemon_num2[m] -= evoluate_num_int;
						evoluate_name_star2 = pokemonDataSystem(have_pokemon[m], "attributes_type",
								0, "", space_charstar2, "main");

						sprintf(evoluate_name,"%s",evoluate_name_star2[0]);
						int find_record = -1;
						for(int n = 0; n <= have_pokemon_attributes; n++){
							if(strcmp(have_pokemon[n], evoluate_name) == 0){
								find_record++;
								have_pokemon_num2[n]++;
								now_pokemon_num = n;
								break;
							}
						}
						if(find_record == -1){
							have_pokemon_attributes++;
							sprintf(have_pokemon[have_pokemon_attributes], "%s",evoluate_name);
							have_pokemon_num2[have_pokemon_attributes] = 1;
							have_pokemon_HP[have_pokemon_attributes] = rand() % 101;
							now_pokemon_num = have_pokemon_attributes;
						}

						severity_level = 0;
						sprintf(logger_message, "%s可以進行升級，系統已經好心的幫你升級了一隻",have_pokemon[m]);
						getLogger(severity_level, "main", logger_message);

						severity_level = 0;
						sprintf(logger_message, "因為升級的關係，背包中已經多出了一隻%s，請好好對待他",have_pokemon[now_pokemon_num]);
						getLogger(severity_level, "main", logger_message);
					}

				}
			}else if(strcmp(input,"5") == 0){
				severity_level = 0;
				sprintf(logger_message, "=======作者資訊=======");
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message, "姓名：鍾騰");
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message, "班級：資工一B");
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message, "學號：410516405");
				getLogger(severity_level, "main", logger_message);
			}else if(strcmp(input,"6") == 0){
				severity_level = 0;
				sprintf(logger_message, "程式即將結束");
				getLogger(severity_level, "main", logger_message);
				break;
			}
		}
		return 0;
	}

	if(strcmp(argv[1],"final_homework") == 0){
		char name_num_char[10];
		char *name_num_char_star = name_num_char;
		int name_num;
		char **name_list;

		struct person_finalhomework person_finalhomework;

		severity_level = 0;
		sprintf(logger_message, "wellcome to play pokemon");
		getLogger(severity_level, "main", logger_message);

//		以時間為種子宣告亂數種子
		srand((unsigned)time(NULL));
		for(;1;){
//			清空標準輸入
			fflush(stdin);

			severity_level = 0;
			sprintf(logger_message,"請輸入選項: ");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(A). 輸入個人資料");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(B). 顯示我的Pokemon種類資料");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(C). 從所有種類中, 找出特殊種類Pokemon");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(D). 創造Pokemon的怪獸(Monster), 進行進化");
			getLogger(severity_level, "main", logger_message);
			sprintf(logger_message,"(E). Exit");
			getLogger(severity_level, "main", logger_message);

			int input_record_int;
			int A_record = 0;
			int B_record = 0;
			int rand_num;

			char input[64];
			gets(input);
			if(strcmp(input,"A") == 0){
				if(A_record){
					severity_level = 2;
					sprintf(logger_message,"A選項已經被執行過了!");
					getLogger(severity_level, "main", logger_message);
					continue;
				}

//				紀錄A已經被選過了
				A_record = 1;

				severity_level = 0;
				sprintf(logger_message,"(A). 輸入個人資料");
				getLogger(severity_level, "main", logger_message);

				sprintf(logger_message,"請輸入你的班級：資工 1.一A 2.一B");
				getLogger(severity_level, "main", logger_message);
				scanf("%d",&input_record_int);

				if(input_record_int == 1){
					sprintf(person_finalhomework.class,"資工一A");
				}else if(input_record_int == 2){
					sprintf(person_finalhomework.class,"資工一B");
				}

				sprintf(logger_message,"請輸入你的姓名");
				getLogger(severity_level, "main", logger_message);
				scanf("%s",person_finalhomework.name);

				sprintf(logger_message,"請輸入你的生日");
				getLogger(severity_level, "main", logger_message);
				scanf("%s",person_finalhomework.date);

				sprintf(logger_message,"請輸入你的電話");
				getLogger(severity_level, "main", logger_message);
				scanf("%s",person_finalhomework.phone_number);

//				準備學號亂數
				rand_num = rand() % 1000000;
				person_finalhomework.studentID = rand_num;

				sprintf(logger_message,"以下是您的使用者資料");
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message,"班級：%s",person_finalhomework.class);
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message,"學號：4105%06d",person_finalhomework.studentID);
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message,"姓名：%s",person_finalhomework.name);
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message,"生日：%s",person_finalhomework.date);
				getLogger(severity_level, "main", logger_message);
				sprintf(logger_message,"電話：%s",person_finalhomework.phone_number);
				getLogger(severity_level, "main", logger_message);

				system("PAUSE");
			}else if(strcmp(input,"B") == 0){
				char file_name[64];
//							   = "./MyPokemon_info_410516405.txt";

//				因應作業要求設定必須先選A選項才能繼續
//				if(A_record){
//					sprintf(logger_message,"您必須先選A選項才能繼續");
//					getLogger(severity_level, "main", logger_message);
//					continue;
//				}

				if(B_record){
					severity_level = 2;
					sprintf(logger_message,"A選項已經被執行過了!");
					getLogger(severity_level, "main", logger_message);
					continue;
				}

				B_record = 1;

				sprintf(logger_message,"請輸入檔案名稱");
				getLogger(severity_level, "main", logger_message);
//				scanf("%s",file_name);
				sprintf(file_name,"MyPokemon_info_410516405.txt");

//				##原本的this_system被當作寫入檔案的名稱
				pokemonDataSystem("", "", space_int, "", space_charstar2,file_name);
//				grading_curve_type當作可否升級依據，1為否、2為是
//				強制進入INITIALIZATION週期
				operatingStateManageSystem(this_system, get_game_state, eventManageSystem_type);
//				獲取寶可夢標籤快取
				pokemonDataSystem("", "", 3, "", space_charstar2, this_system);
				pokemonDataSystem("", "", 2, name_num_char_star, space_charstar2, this_system);
				name_list = pokemonDataSystem("", "", 2, "", space_charstar2, this_system);
				name_num = atoi(name_num_char_star);
				if(name_num == 0){
					severity_level = 3;
					sprintf(logger_message, "There was an error reading the number of name");
					getLogger(severity_level, this_system, logger_message);
				}

				severity_level = 0;
				sprintf(logger_message, "以下是寶可夢名稱和Total");
				getLogger(severity_level, this_system, logger_message);

				for(int m = 0;m <= name_num;m++){
					char luck_record[10];
					char *luck_record_star = luck_record;
					static int run_record = 0;

					pokemonDataSystem(name_list[m], "B.luck", 0,
							luck_record_star, space_charstar2, this_system);

					printf("%s(%s)　",name_list[m],luck_record);
					fflush(stdout);

					run_record++;
					if((run_record % 4) == 0){
						printf("\n");
						fflush(stdout);
					}
				}
				printf("\n");
				fflush(stdout);
			}else if(strcmp(input,"C") == 0){
				int MaxHP,minHP;
				int pokemonID_recordA,pokemonID_recordB,pokemonID_recordC;

				severity_level = 0;
				sprintf(logger_message,"找出最大HP和最小HP的Pokemon");
				getLogger(severity_level, "main", logger_message);

				for(int m = 0;m <= name_num;m++){
					char pokemonHP[10];
					char *pokemonHP_star = pokemonHP;
					int pokemonHP_int;

					if(m == 0){
						pokemonDataSystem(name_list[m], "B.HP", 0,pokemonHP_star, space_charstar2, this_system);
						MaxHP = atoi(pokemonHP);
						minHP = atoi(pokemonHP);
						severity_level = 0;
						sprintf(logger_message,"Pokemon %s的HP %d，是初始比較數值",name_list[m],
								MaxHP);
						getLogger(severity_level, "main", logger_message);
						continue;
					}
					pokemonDataSystem(name_list[m], "B.HP", 0,pokemonHP_star, space_charstar2, this_system);
					pokemonHP_int = atoi(pokemonHP);

					if(pokemonHP_int > MaxHP){
						severity_level = 0;
						sprintf(logger_message,"Pokemon %s的HP %d，比原本最高數值%d還要高，代換MaxHP",name_list[m],
								pokemonHP_int,MaxHP);
						getLogger(severity_level, "main", logger_message);

						MaxHP = pokemonHP_int;
						pokemonID_recordA = m;
					}
					if(pokemonHP_int < minHP){
						severity_level = 0;
						sprintf(logger_message,"Pokemon %s的HP %d，比原本最低數值%d還要低，代換minHP",name_list[m],
								pokemonHP_int,MaxHP);
						getLogger(severity_level, "main", logger_message);

						minHP = pokemonHP_int;
						pokemonID_recordB = m;
					}

				}

				severity_level = 0;
				sprintf(logger_message,"最大HP的Pokemon是%s，他的HP有%d",name_list[pokemonID_recordA],MaxHP);
				getLogger(severity_level, "main", logger_message);

				severity_level = 0;
				sprintf(logger_message,"最小HP的Pokemon是%s，他的HP有%d",name_list[pokemonID_recordB],minHP);
				getLogger(severity_level, "main", logger_message);


			}
		}



	}

	areaDataSystem("", "", space_int, "", space_charstar2,this_system);
	operatingStateManageSystem(this_system, get_game_state, eventManageSystem_type);
	char **test;
	test = areaDataSystem("the main area", "subordinate_area", 0, "",space_charstar2,this_system);

	for(int m = 0;m < 4;m++){
		sprintf(logger_message, "The test char is %s", test[m]);
		getLogger(severity_level, this_system, logger_message);
	}

//	if(skillDataSystem("","", space_int, "", &space_charstar2) == -1)return 1;
//	if(pokemonDataSystem("","", space_int, "", &space_charstar2) == -1)return 1;
//	if(propsDataSystem("","", space_int, "", &space_charstar2) == -1)return 1;
//	if(npcDataSystem("","", space_int, "", &space_charstar2) == -1)return 1;
//	if(triggerEventDataSystem("","", space_int, "", &space_charstar2) == -1)return 1;

//	if(readTheConfiguration("pokemon_data_system") == -1){
//		if(initializationErrorMessager(this_system, "pokemon_data_system") == 1)return 0;
//	}
//
//	if(readTheConfiguration("skillDataSystem") == -1){
//			if(initializationErrorMessager(this_system, "pokemon_data_system") == 1)return 0;
//	}
//
//	if(readTheConfiguration("propsDataSystem") == -1){
//			if(initializationErrorMessager(this_system, "pokemon_data_system") == 1)return 0;
//	}
//
//	if(readTheConfiguration("npcDataSystem") == -1){
//			if(initializationErrorMessager(this_system, "pokemon_data_system") == 1)return 0;
//	}

//	測試資料讀入
//	char nametest[] = "number of loadings";
//	char nametest1[] = "1";
//	char nametest2[] = "name";
//	char nametest3[] = "the main area";
//	char nametest4[] = "subordinate_area";
//	char nametest5[] = "地區1";
//	char nametest6[] = "subordinate_area";
//	char nametest7[] = "地區2";
//	char nametest8[] = "subordinate_area_num";
//	char nametest9[] = "2";
//
//	areaDataSystem(nametest,nametest1);
//	areaDataSystem(nametest2,nametest3);
//	areaDataSystem(nametest8, nametest9);
//	areaDataSystem(nametest4,nametest5);
//	areaDataSystem(nametest6,nametest7);

//	測試中文編碼
	char b[] = "世界";
	sprintf(logger_message, "The char is %s", b);
	getLogger(severity_level, this_system, logger_message);
//	測試輸出延遲
	int a;
	scanf("%d", &a);
	sprintf(logger_message, "The number is %d", a);
	getLogger(severity_level, this_system, logger_message);
	/*廢棄區:設置讀入參考
	 int n = 10;
	 puts("!!!Hello World!!!");
	 char buf[64];
	 char *delim = "=";
	 char s[64];
	 char v;
	 char *p;
	 FILE *fp = fopen("./sit.txt", "r");
	 if(fp == NULL){
	 puts("can not open the file");
	 }
	 p = fgets(buf, 64 ,fp);
	 //	謹記，debug時檔案必須放在專案子目錄中，僅執行.exe則放置在該.exe相同目錄下
	 strcpy(s, p);
	 p = strtok(s, delim);
	 if(p)
	 printf("%s",p);
	 while ((p = strtok(NULL, delim)))
	 printf("%s ", p);
	 printf("\n");

	 char *now_time = getTime();
	 printf("%s",now_time); */
}

#pragma once
#include "PMDG_NGX_SDK.h"
#include "PMDG_NGX_SDK_ADD.h"
#include <array>
#include <vector>


//Перечисление идентификаторов запросов к SimConnect
static enum DATA_REQUESTS_ID {
	PMDG_DATA_REQUEST, //Запрос данных B737NGX
	ADD_DATA_REQUEST, //Запрос дополнительных данных, формируемых LED.DLL
	CONTROL_REQUEST, //Запрос контроля B737NGX
	AIR_PATH_REQUEST, //Запрос пути к файлу *.air загруженного самолета
	REQ_AIRCRAFT_USER, //Запрос переменных необходимых для программы во всех режимах полета
	REQ_SPEEDS, //Запрос данных скорости
	REQ_THRUST, //Запрос данных тяги
	REQ_ELEVATOR //Запрос положения руля высоты
};


//Перечисление идентификаторов запрашиваемых данных
static enum DATA_DEFINITION_IDS {
	DEF_AIRCRAFT_USER, //Структура aircraftData необходимая для программы во всех режимах полета
	DEF_SPEEDS, //Структура tspeeds данных скорости
	DEF_THRUST, //Структура tthrust данных тяги
	DEF_THROTTLE, //Структура tthrottle данных РУД
	DEF_ELEVATOR, //Переменная положения руля высоты
	DEF_AILERON, ////Переменная положения руля крена
	DEF_SPOILER,
	DEF_BRAKE
};


//Пречисление событий формируемых программой, либо симулятором
static enum EVENT_ID {
	EVENT_SIM_START,	// Сим загружен

	

	EVENT_KEYBOARD_A, //Нажата комбинация ctrl+a
	EVENT_KEYBOARD_B, //Нажата комбинация ctrl+b
	EVENT_KEYBOARD_C, //Нажата комбинация ctrl+c
	EVENT_KEYBOARD_D, //Нажата комбинация ctrl+d

	/*EVENT_THROTTLE_SET, //Установка РУД
	EVENT_RUDDER_SET, //Установка руля направления
	EVENT_ELEVATOR_SET, //Установка руля высоты
	EVENT_AILERON_SET, //Установка руля крена
	EVENT_TOGA, //Включение режима TO/GA взлета/ухода

	//PMDG Events
	EVENT_CONTROL_STAND_FLAPS_LEVER_30, //Установка положения закрылок 30 градусов
	EVENT_CONTROL_STAND_FLAPS_LEVER_20, //Установка положения закрылок 5 градусов
	EVENT_CONTROL_STAND_FLAPS_LEVER_15, //Установка положения закрылок 5 градусов
	EVENT_CONTROL_STAND_FLAPS_LEVER_5, //Установка положения закрылок 5 градусов
	EVENT_CONTROL_STAND_FLAPS_LEVER_1, //Установка положения закрылок 1 градус
	EVENT_CONTROL_STAND_FLAPS_LEVER_0, //Установка положения закрылок 0 градусов
	EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN,
	EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP,
	EVENT_LOGO_LIGHT_SWITCH, //Осталось от СДК
	EVENT_FLIGHT_DIRECTOR_SWITCH, //Осталось от СДК
	EVENT_HEADING_SELECTOR, //Осталось от СДК*/

	

	EVENT_TEXT
};

//Перечисление групп событий ввода
static enum INPUT_ID {
	INPUT0
};
//Перечисление групп для подписки на события
static enum GROUP_ID {
	GROUP_KEYBOARD,		// Нажатие клавиш
	GROUP_OTHER, // Прочие события
	GROUP_ELEVATOR, // Событие перестановки руля высоты
	GROUP_PMDG
};
//+++++++++++++ MapClientDataToPMDG ++++++++++++//
static class MAP_CLIENT_DATA_TO_PMDG
{
public:
	const char * szClientDataName; 
	SIMCONNECT_CLIENT_DATA_ID ClientDataID;
	SIMCONNECT_CLIENT_DATA_DEFINITION_ID DefineID;
	DWORD dwOffset;
	DWORD dwSizeOrType;
	//void* pointToStruct;
	float fEpsilon = 0;
	DWORD DatumID;
};

static const std::vector<MAP_CLIENT_DATA_TO_PMDG> MapClientDataToPMDG {
	{ PMDG_NGX_DATA_NAME, PMDG_NGX_DATA_ID, PMDG_NGX_DATA_DEFINITION, 0, sizeof(PMDG_NGX_Data), 0, 0 },
	{ PMDG_ADD_DATA_NAME, PMDG_ADD_DATA_ID, PMDG_ADD_DATA_DEFINITION, 0, sizeof(PMDG_ADD_Data), 0, 0 },
	{ PMDG_NGX_CONTROL_NAME, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION, 0, sizeof(PMDG_NGX_Control), 0, 0 } //Иaybe not useful
};



#pragma once
#include "PMDG_NGX_SDK.h"
#include "PMDG_NGX_SDK_ADD.h"
#include <array>
#include <vector>


//������������ ��������������� �������� � SimConnect
static enum DATA_REQUESTS_ID {
	PMDG_DATA_REQUEST, //������ ������ B737NGX
	ADD_DATA_REQUEST, //������ �������������� ������, ����������� LED.DLL
	CONTROL_REQUEST, //������ �������� B737NGX
	AIR_PATH_REQUEST, //������ ���� � ����� *.air ������������ ��������
	REQ_AIRCRAFT_USER, //������ ���������� ����������� ��� ��������� �� ���� ������� ������
	REQ_SPEEDS, //������ ������ ��������
	REQ_THRUST, //������ ������ ����
	REQ_ELEVATOR //������ ��������� ���� ������
};


//������������ ��������������� ������������� ������
static enum DATA_DEFINITION_IDS {
	DEF_AIRCRAFT_USER, //��������� aircraftData ����������� ��� ��������� �� ���� ������� ������
	DEF_SPEEDS, //��������� tspeeds ������ ��������
	DEF_THRUST, //��������� tthrust ������ ����
	DEF_THROTTLE, //��������� tthrottle ������ ���
	DEF_ELEVATOR, //���������� ��������� ���� ������
	DEF_AILERON, ////���������� ��������� ���� �����
	DEF_SPOILER,
	DEF_BRAKE
};


//����������� ������� ����������� ����������, ���� �����������
static enum EVENT_ID {
	EVENT_SIM_START,	// ��� ��������

	

	EVENT_KEYBOARD_A, //������ ���������� ctrl+a
	EVENT_KEYBOARD_B, //������ ���������� ctrl+b
	EVENT_KEYBOARD_C, //������ ���������� ctrl+c
	EVENT_KEYBOARD_D, //������ ���������� ctrl+d

	/*EVENT_THROTTLE_SET, //��������� ���
	EVENT_RUDDER_SET, //��������� ���� �����������
	EVENT_ELEVATOR_SET, //��������� ���� ������
	EVENT_AILERON_SET, //��������� ���� �����
	EVENT_TOGA, //��������� ������ TO/GA ������/�����

	//PMDG Events
	EVENT_CONTROL_STAND_FLAPS_LEVER_30, //��������� ��������� �������� 30 ��������
	EVENT_CONTROL_STAND_FLAPS_LEVER_20, //��������� ��������� �������� 5 ��������
	EVENT_CONTROL_STAND_FLAPS_LEVER_15, //��������� ��������� �������� 5 ��������
	EVENT_CONTROL_STAND_FLAPS_LEVER_5, //��������� ��������� �������� 5 ��������
	EVENT_CONTROL_STAND_FLAPS_LEVER_1, //��������� ��������� �������� 1 ������
	EVENT_CONTROL_STAND_FLAPS_LEVER_0, //��������� ��������� �������� 0 ��������
	EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN,
	EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP,
	EVENT_LOGO_LIGHT_SWITCH, //�������� �� ���
	EVENT_FLIGHT_DIRECTOR_SWITCH, //�������� �� ���
	EVENT_HEADING_SELECTOR, //�������� �� ���*/

	

	EVENT_TEXT
};

//������������ ����� ������� �����
static enum INPUT_ID {
	INPUT0
};
//������������ ����� ��� �������� �� �������
static enum GROUP_ID {
	GROUP_KEYBOARD,		// ������� ������
	GROUP_OTHER, // ������ �������
	GROUP_ELEVATOR, // ������� ������������ ���� ������
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
	{ PMDG_NGX_CONTROL_NAME, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION, 0, sizeof(PMDG_NGX_Control), 0, 0 } //�aybe not useful
};



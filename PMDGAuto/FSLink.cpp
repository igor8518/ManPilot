#include "FSLink.h"
#include "MainForm.h"


//using namespace PMDGAuto;


namespace PMDGAuto
{
	using CSWrap::Util;
	using namespace System;
	
	FSLink::FSLink()
	{
		//HRESULT hr;
		quit = 0;
		if ((!Connected))
		{
			MForm->SetConnectState(CONNECTING);
			MForm->Refresh();
			try
			{
				if (SUCCEEDED(SimConnect_Open(&hSimConnect, "PMDG NGX Auto", NULL, 0, 0, 0)))
				{
					MForm->SetConnectState(CONNECTED);
					MForm->Refresh();
					TESTHR(SimConnect_AttachObjectToSimObject(hSimConnect, 0, { 0, 0, 0 }, { 0, 0, 0 }, "Boeing 737-84R NGX Nordstar VQ-BNG Winglets", { 0, 0, 0 }, { 0, 0, 0 }, 0),"SimConnect_AttachObjectToSimObject");
					InitSimConnect();
				}
				else
				{
					MForm->SetConnectState(DISCONNECTED);
				};
			}
			catch (...)
			{
				
			}
		}
		else
		{
			MForm->SetConnectState(DISCONNECTED);
		}
	}
	FSLink::~FSLink()
	{
	}
	
	HRESULT FSLink::InitSimConnect()
	{
		try
		{		
			const char* ss = Util::GetPMDGEventNameFromID(EVT_OH_LIGHTS_LOGO);
			for (int i = 0; i<MapClientDataToPMDG.size();i++)			
			{
				// Associate an ID with the PMDG data area name
				TESTHR(SimConnect_MapClientDataNameToID(hSimConnect, 
					MapClientDataToPMDG[i].szClientDataName,
					MapClientDataToPMDG[i].ClientDataID),
					"SimConnect_MapClientDataNameToID");
				// Define the data area structure - this is a required step
				TESTHR(SimConnect_AddToClientDataDefinition(hSimConnect,
					MapClientDataToPMDG[i].DefineID,
					MapClientDataToPMDG[i].dwOffset,
					MapClientDataToPMDG[i].dwSizeOrType,
					MapClientDataToPMDG[i].fEpsilon,
					MapClientDataToPMDG[i].DatumID),
					"SimConnect_AddToClientDataDefinition");
				// Sign up for notification of data change.  
				// SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED flag asks for the data to be sent only when some of the data is changed.
				TESTHR(SimConnect_RequestClientData(hSimConnect, 
					MapClientDataToPMDG[i].ClientDataID, i, 
					MapClientDataToPMDG[i].DefineID,
					SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, 
					SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0),
					"SimConnect_RequestClientData");
			}
			for (int i = 0; i < PMDGEventArray.size(); i++)
			{
				//TESTHR(Util::SimConnect_MapClientEventToPMDGEvent(hSimConnect, PMDGEventArray[i].eventID), "SimConnect_MapClientEventToSimEvent");
				TESTHR(SimConnect_MapClientEventToSimEvent(hSimConnect, i, Util::GetPMDGEventNameFromID(PMDGEventArray[i].eventID)), "SimConnect_MapClientEventToSimEvent");
				TESTHR(SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_PMDG, i), "SimConnect_AddClientEventToNotificationGroup");
			}
			TESTHR(SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_PMDG, SIMCONNECT_GROUP_PRIORITY_HIGHEST), "SimConnect_SetNotificationGroupPriority");		
			//TESTHR(SimConnect_CallDispatch(hSimConnect, ReceiveCallBack, NULL), "SimConnect_CallDispatch");		
			
			Timer = SetTimer(NULL, 0, 63, TimerProc);
		}
		catch (HRESULT &hr)
		{
			return hr;
		}
		return NOERROR;
	}
	void CALLBACK FSLink::TimerProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ UINT_PTR idEvent, _In_ DWORD dwTime)
	{
		static bool InTimer;
		if (!InTimer)
		{
			InTimer = true;
			TESTHR(MainForm::PFSLink->Dispatch(), "SimConnect_CallDispatch");
			InTimer = false;
		}
	}
	delegate void SetText(String^ text, bool simDisplay);
	void AddText(String^ text, bool simDisplay = false)
	{
		if (MainForm::PMainForm->testEventsForm->outBox->InvokeRequired)
		{
			SetText^ d = gcnew SetText(AddText);
			MainForm::PMainForm->testEventsForm->Invoke(d, gcnew array<Object^> {text, simDisplay});
		}
		else
		{
			MainForm::PMainForm->testEventsForm->outBox->AppendText(text);
		}
	}
	delegate void SetChanged(String^ text, int Timer);
	void AddChanged(String^ text, int Timer)
	{
		if (MainForm::PMainForm->testEventsForm->ChangedVarsList->InvokeRequired)
		{
			SetChanged^ d = gcnew SetChanged(AddChanged);
			MainForm::PMainForm->testEventsForm->Invoke(d, gcnew array<Object^> {text, Timer});
		}
		else
		{
			MainForm::PMainForm->testEventsForm->ChangedVarsList->Items->Insert(0,text);
			//MainForm::PMainForm->testEventsForm->ChangedVarsList->Items->(0, text);
			//MainForm::PMainForm->testEventsForm->ChangedVarsList->Items[0]->
		}
	}
	void ClearChanged(String^ text, bool simDisplay = false)
	{
		if (MainForm::PMainForm->testEventsForm->ChangedVarsList->InvokeRequired)
		{
			SetText^ d = gcnew SetText(ClearChanged);
			MainForm::PMainForm->testEventsForm->Invoke(d, gcnew array<Object^> {text, simDisplay});
		}
		else
		{
			MainForm::PMainForm->testEventsForm->ChangedVarsList->Items->Clear();
		}
	}
	void FSLink::Process(SIMCONNECT_RECV *pData, DWORD cbData)
	{
		
		switch (pData->dwID)
		{
		case SIMCONNECT_RECV_ID_CLIENT_DATA:
		{
			SIMCONNECT_RECV_CLIENT_DATA *clData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;
			switch (clData->dwRequestID)
			{
			case PMDG_DATA_REQUEST:
			{
				//AddText(DateTime::Now.ToLongTimeString() + " - " + clData->dwID.ToString() + " | " + clData->dwDefineID.ToString() + "\n");
				PMDG_NGX_Data* data = (PMDG_NGX_Data*)&clData->dwData;
				
				//std::vector<char>* dataArray = (std::vector<char>*)&clData->dwData;
				//dataArray->resize(sizeof(PMDG_NGX_Data));
				void* dataArray = (void*)&clData->dwData;
				//std::vector<PMDG_DATA_TABLE>* dataArray = (std::vector<PMDG_DATA_TABLE>*)&clData->dwData;
				byte* currPoint = (byte*)dataArray;
				int currArrayPoint = 0;
				int k = 0;
				if (firstReceive)
				{
					for (int i = 0; i < sizeof(PMDG_NGX_Data)-156; i++)
					{
						
						if ((PMDGDataTable[k].size != 1) &&
							(strcmp(PMDGDataTable[k].szClientTypeName, "char") != 0)
							)
						{
							int align = (i%PMDGDataTable[k].size);
							if (align != 0)
							{
								currPoint = (byte*)((unsigned long long)currPoint + PMDGDataTable[k].size - align);
								i = i + PMDGDataTable[k].size - align;
							}
						}
						PMDGDataTable[k].endOffset = i;
						for (int j = 0; j < PMDGDataTable[k].size; j++)
						{
							savedArray[i+j] = *currPoint;
							savedArrayVars[i + j] = k;
						}
						i = i + PMDGDataTable[k].size - 1;
						currPoint = (byte*)((unsigned long long)(dataArray) + (unsigned long long)(i + 1));
						k++;
						if (k >= PMDGDataTable.size())
						{
							k = PMDGDataTable.size() - 1;
						}
						//currPoint = (byte*)((unsigned long long)currPoint + 1);
					}
					firstReceive = false;
					
				}
				currPoint = (byte*)dataArray;
				k = 0;
				
				for (int i = 0; i < sizeof(PMDG_NGX_Data)-156; i++)
				{
					int f = -1;
					int test = sizeof(data->FUEL_FuelTempNeedle);
					void* test_7 = &data->FUEL_FuelTempNeedle;
					/*void* test_7 = &data->ENG_StartValve[0];
					void* test_6 = &data->ENG_StartValve[1];
					void* test_5 = &data->AIR_DuctPress[0];
					void* test_4 = &data->AIR_DuctPress[1];
					void* test_3 = &data->COMM_Attend_PressCount;
					void* test_2 = &data->COMM_GrdCall_PressCount;
					void* test_1 = &data->COMM_SelectedMic[0];
					void* test0 = &data->COMM_SelectedMic[1];
					void* test1 = &data->COMM_SelectedMic[2];
					void* test2 = &data->FUEL_QtyCenter;
					void* test3 = &data->FUEL_QtyLeft;*/
					void* test1 = &data->MAIN_annunLE_FLAPS_EXT;
					void* test2 = &data->MAIN_TEFlapsNeedle[0];
					void* test3 = &data->MAIN_TEFlapsNeedle[1];
					void* test4 = &data->MAIN_annunGEAR_transit[0];
					void* test5 = &data->MAIN_annunGEAR_transit[1];
					void* test6 = &data->MAIN_annunGEAR_transit[2];
					void* test7 = &data->MAIN_annunGEAR_locked[0];
					void* test8 = &data->MAIN_annunGEAR_locked[1];
					void* test9 = &data->MAIN_annunGEAR_locked[2];
					void* test10 = &data->MAIN_GearLever;
					void* test11 = &data->MAIN_BrakePressNeedle;
					void* test12 = &data->HGS_annun_AIII;
					if ((PMDGDataTable[k].size != 1)&&
						(strcmp(PMDGDataTable[k].szClientTypeName,"char")!=0)
						//&&(strcmp(PMDGDataTable[k].szClientTypeName, "float[]")!=0)
						//&& (strcmp(PMDGDataTable[k].szClientTypeName, "unsigned int[]") != 0)
						)
					{
						int align = (i%PMDGDataTable[k].size);
						if (align != 0)
						{
							currPoint = (byte*)((unsigned long long)currPoint + PMDGDataTable[k].size - align);
							i = i + PMDGDataTable[k].size - align;
						}
					}
					for (int j = 0; j < PMDGDataTable[k].size; j++)
					{
						
						if (savedArray[i+j] != *currPoint)
						{
							//savedArray[i + j] = *currPoint;
							if (f != k)
							{
								AddText(DateTime::Now.ToLongTimeString() + " ------ " + gcnew System::String(PMDGDataTable[k].szClientDataName));
								if (strcmp(PMDGDataTable[k].szClientTypeName,"unsigned char")==0)
								{
									AddText(" = " + *(unsigned char*)&(savedArray[i]) + " => " + *(unsigned char*)(currPoint) + "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName,"bool") == 0)
								{
									AddText(" = " + *(bool*)&(savedArray[i]) + " => " + *(bool*)(currPoint)+ "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName,"float") == 0)
								{
									AddText(" = " + *(float*)&(savedArray[i]) + " => " + *(float*)(currPoint)+ "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName, "float[]") == 0)
								{
									AddText(" = " + *(float*)&(savedArray[i]) + " => " + *(float*)(currPoint)+ "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName, "unsigned int") == 0)
								{
									AddText(" = " + *(unsigned int*)&(savedArray[i]) + " => " + *(unsigned int*)(currPoint)+ "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName, "unsigned int[]") == 0)
								{
									AddText(" = " + *(unsigned int*)&(savedArray[i]) + " => " + *(unsigned int*)(currPoint)+ "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName,"unsigned short") == 0)
								{
									AddText(" = " + *(unsigned short*)&(savedArray[i]) + " => " + *(unsigned short*)(currPoint)+ "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName, "short") == 0)
								{
									AddText(" = " + *(short*)&(savedArray[i]) + " => " + *(short*)(currPoint)+ "\n");
								}
								else if (strcmp(PMDGDataTable[k].szClientTypeName, "char") == 0)
								{
									String^ ssOld;
									String^ ssNew;
									for (int is = 0; is < PMDGDataTable[k].size; is++)
									{
										if (savedArray[i + is] != '\0')
										{
											ssOld += wchar_t(savedArray[i + is]);

										}
										if ((char*)currArrayPoint != '\0')
										{
											ssNew += wchar_t(currArrayPoint);

										}
									}

									//ss = gcnew System::String((char*)savedArray[i]);
									AddText(" = " + ssOld + " - >" + ssNew + "\n");
									
								}
								if (!currentChangedValue.empty())
								{
									it = std::find(currentChangedValue.begin(), currentChangedValue.end(), i);
									if (it == currentChangedValue.end())
									{
										currentChangedValue.push_back(i);
										currentChangedCounter.push_back(25);
									}
									else
									{
										int dist = std::distance(currentChangedValue.begin(), it);
										currentChangedCounter[dist] = 25;
									}
								}
								else
								{
									currentChangedValue.push_back(i);
									currentChangedCounter.push_back(25);
								}
							}
							savedArray[i + j] = *currPoint;
							
							
							
							f = k;
							//break;
						}
						currPoint = (byte*)((unsigned long long)currPoint + 1);
					}
					
					//if (currentChangedValue.size() != 0)
					
					i = i + PMDGDataTable[k].size-1;
					currPoint = (byte*)((unsigned long long)(dataArray) + (unsigned long long)(i+1));
					k++;
					if (k >= PMDGDataTable.size())
					{
						k = PMDGDataTable.size() - 1;
					}
						
				}

				
				break;
			}
			case CONTROL_REQUEST:
			{
				PMDG_NGX_Control* control = (PMDG_NGX_Control*)&clData->dwData;
				//AddText(DateTime::Now.ToLongTimeString() + " - " + control->Event + " | Parameter = " + control->Parameter + "\n");
				//break;
				int i;
				for (i = 0; i < PMDGEventArray.size(); i++)
				{
					if (control->Event == PMDGEventArray[i].eventID)
					{
						break;
					}
				}
				
				int j;
				for (j = 0; j < PMDGMouseEvents.size(); j++)
				{
					if (control->Parameter == PMDGMouseEvents[j].mouseEventID)
					{
						break;
					}
				}
				if (j == PMDGMouseEvents.size())
				{
					if (i == PMDGEventArray.size())
					{
						AddText(DateTime::Now.ToLongTimeString() + " - " + control->Event + " | " + control->Parameter + "\n");
					}
					else
					{
						AddText(DateTime::Now.ToLongTimeString() + " - " + gcnew System::String(PMDGEventArray[i].szEventName) + " | " + control->Parameter + "\n");
					}
				}
				else
				{
					if (i == PMDGEventArray.size())
					{
						AddText(DateTime::Now.ToLongTimeString() + " - " + control->Event + " | " + gcnew System::String(PMDGMouseEvents[j].szmouseEventName) + "\n");
					}
					else
					{
						AddText(DateTime::Now.ToLongTimeString() + " - " + gcnew System::String(PMDGEventArray[i].szEventName) + " | " + gcnew System::String(PMDGMouseEvents[j].szmouseEventName) + "\n");
					}
				}
				break;
			}
			default:
			{
				AddText(DateTime::Now.ToLongTimeString() + " - " + clData->dwID.ToString() + " | " + clData->dwDefineID.ToString() + "\n");
				break;
			}
			}
			break;
		}
		case SIMCONNECT_RECV_ID_EVENT:
		{
			SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;
			if (evt->uEventID <= (PMDGEventArray.size() - 1) &&
				(evt->dwData != 0x00400000))
			{
				int i;
				for(i = 0; i < PMDGMouseEvents.size(); i++)
				{
					if (evt->dwData == PMDGMouseEvents[i].mouseEventID)
					{
						break;
					}
				}
				if (i == PMDGMouseEvents.size())
				{
					AddText(DateTime::Now.ToLongTimeString() + " - " + gcnew System::String(PMDGEventArray[evt->uEventID].szEventName) + " | " + evt->dwData.ToString()+"\n");
				}
				else
				{
					AddText(DateTime::Now.ToLongTimeString() + " - " + gcnew System::String(PMDGEventArray[evt->uEventID].szEventName) + " | " + gcnew System::String(PMDGMouseEvents[i].szmouseEventName) + "\n");
				}
			}
			else if(evt->dwData != 0x00400000)
			{
				AddText(DateTime::Now.ToLongTimeString() + " - " + evt->uEventID.ToString() + " | " + evt->dwData.ToString() + "\n");
			}
			//MessageBoxEx(0, "123", "111", 0, 0);
			break;
		}
		case SIMCONNECT_RECV_ID_QUIT:
		{
			MainForm::PFSLink->quit = 1;
			break;
		}
		case SIMCONNECT_RECV_ID_EXCEPTION:
		{
			SIMCONNECT_RECV_EXCEPTION *exc = (SIMCONNECT_RECV_EXCEPTION*)pData;
			AddText(DateTime::Now.ToLongTimeString() + " - " + exc->dwID.ToString() + " | " + exc->dwException.ToString() + "\n");
			break;
		}
		default:
		{
			AddText(DateTime::Now.ToLongTimeString() + " - " + pData->dwID.ToString()+ "\n");
			break;
		}
		}
	}
	HRESULT FSLink::Dispatch()
	{
		ClearChanged(""); //Убрать лишние параметры
		for (int n = currentChangedValue.size() - 1; n >= 0; n--)
		{
			currentChangedCounter[n] = currentChangedCounter[n] - 1;
			if (currentChangedCounter[n] <= 0)
			{
				currentChangedCounter.erase(currentChangedCounter.begin() + n);
				currentChangedValue.erase(currentChangedValue.begin() + n);
			}
			else
			{
				//currentChangedCounter[i] = currentChangedCounter[i] - 1;
				//AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName), currentChangedCounter[n]);
				//AddText(DateTime::Now.ToLongTimeString() + " ------ " + gcnew System::String(PMDGDataTable[k].szClientDataName));
				if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "unsigned char") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(unsigned char*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "bool") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(bool*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "float") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(float*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "float[]") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(float*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "unsigned int") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(unsigned int*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "unsigned int[]") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(unsigned int*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "unsigned short") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(unsigned short*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "short") == 0)
				{
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + *(short*)&(savedArray[currentChangedValue[n]]) + "\n", currentChangedCounter[n]);
				}
				else if (strcmp(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientTypeName, "char") == 0)
				{
					//String^ ssOld;
					String^ ssNew;
					for (int is = 0; is < PMDGDataTable[savedArrayVars[currentChangedValue[n]]].size; is++)
					{
						/*if (savedArray[currentChangedValue[n] + is] != '\0')
						{
						ssOld += wchar_t(savedArray[currentChangedValue[n] + is]);

						}*/
						if (savedArray[currentChangedValue[n] + is] != '\0')
						{
							ssNew += wchar_t(savedArray[currentChangedValue[n] + is]);

						}
					}

					//ss = gcnew System::String((char*)savedArray[i]);
					AddChanged(gcnew System::String(PMDGDataTable[savedArrayVars[currentChangedValue[n]]].szClientDataName) + " = " + ssNew + "\n", currentChangedCounter[n]);

				}

			}
		}
		return ::SimConnect_CallDispatch(hSimConnect, &FSLink::ReceiveCallBack, this);
		
	}

	// static function
	void CALLBACK FSLink::ReceiveCallBack(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
	{
		FSLink *PFSLink = reinterpret_cast<FSLink*>(pContext);
		PFSLink->Process(pData, cbData);
	}
	
	
}


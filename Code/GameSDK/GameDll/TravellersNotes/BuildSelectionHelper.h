#ifndef _MySelectionBuildHelper_
#define _MySelectionBuildHelper_
#include "Game.h"
#include "IHardwareMouse.h"
#include "EconomicResources.h"

//� ������ ��������� ������� ����, ����������� ��� ���������� � ������������� ���������� ������.
//������ ���� � �������� ����������� � ����-�������������, ��� ������������ �����������.


struct SBuilding
{
	string Name;	//������������ ������
	string Descr;	//�������� ������
	string Additional;	//�������������� ���� �� ������ ������
	string Model;	//���� � ������
	int Type;	//��� ������
	int Cost;	//���� ������
	int Wood;	//����������� ���������� ������
	int Stone;	//���������� �����
	int Iron;	//���������� ������

	std::vector <SResource> ProvidingResources;
	//�������� ��� ������ ������������ �����-�� ���������� �������(������) ��� �������������.
	//��� ����� ������ � �������� ��������� ����, �� ����������� ������, ���-�� �����:
	//int UniqResource;
	//-----------------------------
	 SBuilding()
	 {
		 Type = 0;
		 Cost = 0;
		 Wood = 0;
		 Stone= 0;
		 Iron = 0;
		 //�������� ���������� ������ � �����������.
	 }
};


//����� ��������� �������� ������ ������� ���������
//� ���������� ��� ���������� ��� ���������� �������� ��������� ������ �� ����
//���������: IActionListener - � ������ ������ ������������ ��� �������� ����� �����.
class CSelectionBuildHelper
{
public: 
	CSelectionBuildHelper();

	//��������:
	//������� ���������� �������� ��������� ����� �� 3�-�����, �������� ��������� ��������, ���� �������� ���� �������,
	//��������� ��������� ��������, ���������� �� ������, ������� ������� � ���� ������-������, �������� �,
	//�������� ���� ���� � ��������� ������ ������ �� ����� �� ������� �������.
	void Select();

	//��������:
	//������� ��������� ��������� �� �������� ����� ��������, � �������������� ������ ������ ������ ��� ������� �������.
	void BuildOptions();

	//��������:
	//������� ���������� ����� ������.
	//���������: ���� � ������ ������.
	void SpawnBuilding(string model);

	//������� ���� ���������� ��������� ������ �� �����
	//��������:
	//������ ������� ��������� �� XML ����� ���������, �� ��������� ����� ������.
	//���������: ���������� ��� ������, ������� ����� ����� � ���
	SBuilding* GetBuildParams(const char* name);
	SBuilding* GetBuilding(int index){ return &m_Buildings[index]; }
	
	//��������:
	//������� ���������� �������� ����� �� 2�-������ � 3�-����� � ���������� ��������, �� ������� �������� �����
	IEntity* GetMouseEntityID();
private:
	IEntity* m_SelectedEntityBuildId;
	std::vector<SBuilding> m_Buildings;//������ ������ ����������� ������
};


#endif
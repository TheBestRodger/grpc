#pragma once

#pragma pack(push, 1)

struct OESKN_in
{
	// (��� ������-2)
	// ������� ��������� �� ������� ���������� ������� [�]
	unsigned int distance{};	//Controlsys RVector r

	// (��� ������-2) ���� �������� ������� [���]
	double gam_i{};				//TmData R_et, psi_et,R_et.y

	// ���������� ������� struct mmdata
	double B{};					// ������  [��] mislemotion b
	double L{};					// ������� [��] b
	double H{};					// ������  [�]  R.y

	// ������� ������� (�������� ����� ��������) mislemotion
	double tet{};				// ���� �������  [���] orn.t
	double psi{};				// ���� �������� [���] orn.p
	double gam{};				// ���� �����    [���] orn.g

	// ������������ (��������� ����� �������� ��� ������-2) HHdata
	double fiv_z{};				// ������������ ������ ����   [���] phig
	double fig_z{};				// �������������� ������ ���� [���] phiv

	// ����� ������:
	// == 0 - ��������
	// != 0 - �������
	int mode{};

	// ������� �������
	int frontier{};				// ����� �� �����
};

struct Picture_out
{
	std::vector<uint8_t> pix;
	unsigned size_pix{};

	unsigned height{};
	unsigned width{};
};

struct OESKN_out
{
	// ���� �������� ������������ (��������� ����� �������� ��� ������-2)
	double fiv{};				// ������������ ����   [���]
	double fig{};				// �������������� ���� [���]

	// �������� �������� ������������
	double omv{};				// ������������   [���/�]
	double omg{};				// �������������� [���/�]

	// ���� ������� ���� � ��������� ��
	double fiv_c{};				// ������������ ����   [���]
	double fig_c{};				// �������������� ���� [���]

	// ������� ������� ����
	int ZG{};
};

#pragma pack(pop)
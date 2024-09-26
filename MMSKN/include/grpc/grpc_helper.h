#pragma once

#include <memory>
#include <string>

#include "../IOStructs.h"

/// <summary>
/// API для взаимодействия по протоколу gRPC
/// </summary>
class GrpcHelper
{
public:
	explicit GrpcHelper();
	~GrpcHelper() = default;

	// Параметры подключения к серверу ----------------------

	/// <summary>
	/// Хост
	/// </summary>
	/// <returns>Хост</returns>
	[[nodiscard]] const std::string host() const;
	void setHost(const std::string& host);
	
	/// <summary>
	/// Порт на котором работает сервер
	/// </summary>
	/// <returns>Порт на котором работает сервер</returns>
	[[nodiscard]] int port() const;
	void setPort(int port);

	/// <summary>
	/// Задать параметры для безопасного подключения
	/// </summary>
	/// <param name="useTls"> - использовать безопасное подключение</param>
	/// <param name="serverCertFile"> - файл, содержащий PEM-кодировку корневых сертификатов сервера</param>
	/// <param name="clientKeyFile"> - файл, содержащий PEM-кодировку закрытого ключа клиента</param>
	/// <param name="clientCertFile"> - файл, содержащий PEM-кодировку цепочки сертификатов клиента</param>
	void setSslCredentials(bool useTls, std::string const& serverCertFile,
		std::string const& clientKeyFile, std::string const& clientCertFile);


	// Запросы к серверу ------------------------------------

	/// <summary>
	/// Получить снимок с телевизионной камеры. Запрос синхронный
	/// </summary>
	/// <returns>Полученный снимок или nullptr при ошибке</returns>
	PictureRecieve* recieve_img_TV();

	/// <summary>
	/// Получить снимок с инфракрасной камеры. Запрос синхронный
	/// </summary>
	/// <returns>Полученный снимок или nullptr при ошибке</returns>
	PictureRecieve* recieve_img_IR();

	/// <summary>
	/// Содержание ошибки при выполнении последней операции
	/// </summary>
	/// <returns>Сообщение об ошибке</returns>
	const std::string& lastErrorMessage() const;

private:
	class Data;
	const std::shared_ptr<Data> data;
};


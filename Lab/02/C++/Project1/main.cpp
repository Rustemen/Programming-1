﻿#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include "include/cpp-httplib/httplib.h"
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace httplib;

//std::ofstream logger("output.log");

/* Читает конфигурационный файл с именем из переменной 'config_file_name' и возвращает десериализованный json.
 * Гарантирует, что вернёт {"webhooks": []} в случае ошибки. */
json get_config();


/* Записывает json в конфигурационный файл по возможности. */
void save_config(json config);


/* Читает из файла с именем из переменной 'webpage_file_name' веб-страницу и заменяет в прочитанных данных
 * {webhooks_list} на список вебхуков с кнопками, сделанные по шаблону из переменной 'webhook_template'.
 * Возвращает пустую строку "" в случае ошибки. */
std::string gen_webhook_page();


/* Функции для управления вебхуками через веб-интерфейс. */
void webhooks_get(const Request& req, Response& res);
void webhooks_post(const Request& req, Response& res);


/* Функция для вебхука Алисы. */
void yandex_hook(const Request& req, Response& res);


int main()
{
	// Если компилируется на Windows, сделать русские символы в терминале читабельными.
	// Update: к чёрту эту хрень, переводите терминалы на UTF-8.
	// https://stackoverflow.com/a/57134096

/*#if defined(_WIN32) || defined(WIN32)
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
#endif */

	Server srv;

	// Для проверки пригодится https://webhook.site/
	srv.Get("/webhooks", webhooks_get);
	srv.Post("/webhooks", webhooks_post);

	// Яндекс будет посылать периодические запросы, а так же запросы при изменении состояния чатов.
	srv.Post("/yandex_hook", yandex_hook);

	std::cout << "Сервер успешно запустился!\n"
		<< "Сервер откыт по адресу localhost:1234\n\n"
		<< "Откройте http://localhost:1234/webhooks в веб-браузере, "
		<< "чтобы получить доступ к панели управления веб-хуками.\n"
		<< std::endl;

	srv.listen("localhost", 1234);
}


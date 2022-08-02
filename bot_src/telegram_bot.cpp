
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <ctime>
#include <curl/curl.h>
#include "/home/vladislav/proekt/db_module/db_create_mod.h"
#include "json.hpp"


using  namespace TgBot;
using  namespace std;

vector<string>bot_commands = {"start", "time", "currency","add_transaction"};

string get_time_as_str(){ // функция для получения текущего времени
    time_t now = time(NULL);
    tm* ptr = localtime(&now);
    char buf[32];
    strftime(buf, 32, "%c", ptr);
    return buf;
}
static size_t Writer(char *buffer, size_t size, size_t nmemb, std::string *html){
    int result = 0;

    if (buffer != NULL){
        html->append(buffer, size*nmemb);
        result = size*nmemb;
    }
    return result;
}
std::string get_request(std::string link){
    CURL *curl;
    std::string data;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return data;

}


float get_currency(char what){
    auto js_obj = nlohmann::json::parse(get_request("https://www.cbr-xml-daily.ru/daily_json.js"));
    if (what == 'u') {
        return js_obj["Valute"]["USD"]["Value"].get<float>();
    } else {
        return js_obj["Valute"]["EUR"]["Value"].get<float>();
    }
    return -1;
}


int main() {


    Bot bot("5316353881:AAE_WROfqzYnLKCiD1XX7jFIn19kSWii3OQ");


    // Чтение нажатия на кнопки(создание клавиатуры)
    // currency
    InlineKeyboardMarkup::Ptr  keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> buttons;
    InlineKeyboardButton::Ptr usd_btn(new InlineKeyboardButton), eur_btn(new InlineKeyboardButton);

    usd_btn->text = "USD";
    eur_btn->text = "EUR";

    usd_btn->callbackData = "USD";
    eur_btn->callbackData = "EUR";

    buttons.push_back(usd_btn);
    buttons.push_back(eur_btn);

    keyboard->inlineKeyboard.push_back(buttons);

    // add_transaction
    InlineKeyboardMarkup::Ptr  keyboard2(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> buttons2;
    InlineKeyboardButton::Ptr revenue_btn(new InlineKeyboardButton), expenses_btn(new InlineKeyboardButton);

    revenue_btn->text = "Доход";
    expenses_btn->text = "Расход";

    revenue_btn->callbackData = "revenue";
    expenses_btn->callbackData = "expenses";

    buttons2.push_back(revenue_btn);
    buttons2.push_back(expenses_btn);

    keyboard2->inlineKeyboard.push_back(buttons2);


// установка команд бота (его действия при нажатии на кнопку)

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi, " + message->chat->firstName);
    });
    // команда для получения текущего времени
    bot.getEvents().onCommand("time", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Moscow time:\n " + get_time_as_str());
    });
    // команда для получения курса валют
    bot.getEvents().onCommand("currency", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "How currency?", false, 0, keyboard);
    });
    // команда для записи дохода/расхода
    bot.getEvents().onCommand("add_transaction", [&bot, &keyboard2](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "select the transaction type!", false, 0, keyboard2);
    });


    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query){
        if (query->data == "USD"){
            bot.getApi().sendMessage(query->message->chat->id, to_string(get_currency('u')));
        };
        if (query->data == "EUR"){
            bot.getApi().sendMessage(query->message->chat->id, to_string(get_currency('e')));
        }
    });
    int flag_add = 0;
    int chat_id = 0;
    bot.getEvents().onCallbackQuery([&bot, &keyboard2, &flag_add, &chat_id](CallbackQuery::Ptr query){
        chat_id = query->message->chat->id;
        if (query->data == "revenue"){
            bot.getApi().sendMessage(query->message->chat->id, "Введи доход в рублях");
            flag_add = 1;
        };
        if(query->data == "expenses"){
            bot.getApi().sendMessage(query->message->chat->id, "Введи расход в рублях");
            flag_add = 2;
            }
        });
        
    bot.getEvents().onAnyMessage([&bot, &chat_id, &flag_add](Message::Ptr message) {

        for (const auto& command : bot_commands){
            if ("/" + command == message->text){
               /* switch (command)
                case "add":{

                    break;
                }*/

                return;
            }

            if (double trans = strtod( message->text.c_str(), NULL) && chat_id  && flag_add ){
              trans = strtod( message->text.c_str(), NULL);
              if (flag_add == 2){
                  trans *=-1;
              }
               sqlite3* DB;
               string col_names[] {\
                 "id INTEGER ",\
                "trans REAL ",\
                "date TEXT ",\
                "inf TEXT"};
               sqlite3_open("dat_base.db", &DB);

               create_tabl(DB, "data", 4, col_names);
               string row_values[] {\
                to_string(chat_id),\
                to_string(trans),\
                "date('now','0 month') ",\
                "'inf'"};
                insert_elem(DB, "data", 4, row_values);
                close_db(DB);
                flag_add = 0;
                bot.getApi().sendMessage(chat_id, "Внесено в базу");
                return;
            }
        }
        bot.getApi().sendMessage(message->chat->id, "Sorry, I don't know : (");
    });




    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}

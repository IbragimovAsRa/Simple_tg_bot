
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <ctime>
#include <curl/curl.h>

#include "json.hpp"


using  namespace TgBot;
using  namespace std;

vector<string>bot_commands = {"start", "time", "currency","add"};

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




// установка команд бота (его действия при нажатии на кнопку)

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi, " + message->chat->firstName);
    });
    bot.getEvents().onCommand("time", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Moscow time:\n " + get_time_as_str());
    });
    bot.getEvents().onCommand("currency", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "How currency?", false, 0, keyboard);
    });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query){
        if (query->data == "USD"){
            bot.getApi().sendMessage(query->message->chat->id, to_string(get_currency('u')));
        } else {
            bot.getApi().sendMessage(query->message->chat->id, to_string(get_currency('e')));
        }
    });
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {

        for (const auto& command : bot_commands){
            if ("/" + command == message->text){
               /* switch (command)
                case "add":{

                    break;
                }*/

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

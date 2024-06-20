## Part 1. Настройка gitlab-runner

Поднял виртуальную машину Ubuntu Server 20.04 LTS. Поключился по ssh с помощью Putty.
Скачал и установить на виртуальную машину gitlab-runner

![runner_install_curl](/src/images/runner_install_curl.png)

![runner_install_apt](/src/images/runner_install_apt.png)

Запуститл gitlab-runner и зарегистрировал его для использования в текущем проекте (DO6_CICD)

![register](/src/images/register.png)

## Part 2. Сборка

Написатл этап для CI по сборке приложений из проекта C2_SimpleBashUtils:

В файле gitlab-ci.yml добавитл этап запуска сборки через мейк файл из проекта C2

![runner_install_curl](/src/images/build.png)

![runner_install_curl](/src/images/build_cat.png)

Логи работы пайплайна 

![runner_install_curl](/src/images/build_log.png)

![runner_install_curl](/src/images/build_log_grep.png)

## Part 3. Тест кодстайла

Написатл этап для CI, который запускает скрипт кодстайла (clang-format):

![runner_install_curl](/src/images/codestyle.png)

![runner_install_curl](/src/images/codestyle_sh.png)

В пайплайне отобразитл вывод утилиты clang-format

![runner_install_curl](/src/images/codestyle_log.png)

Заваленный пайплайн

![runner_install_curl](/src/images/codestyle_failed_log.png)

## Part 4. Интеграционные тесты

Написатл этап для CI, который запускает интеграционные тесты:

![runner_install_curl](/src/images/integration.png)

![runner_install_curl](/src/images/integration_sh.png)

В пайплайне отобразил вывод, что интеграционные тесты успешно прошли / провалились

![runner_install_curl](/src/images/integration_log.png)

![runner_install_curl](/src/images/integration_log_grep.png)

![runner_install_curl](/src/images/pipeline.png)

## Part 5. Этап деплоя

Поднятл вторую виртуальную машину Ubuntu Server 20.04 LTS.
Написатл этап для CD, который "разворачивает" проект на другой виртуальной машине:

Написатл bash-скрипт, который при помощи ssh и scp копирует файлы, полученные после сборки (артефакты), в директорию /usr/local/bin второй виртуальной машины

![runner_install_curl](/src/images/deploy_sh.png)

В файле gitlab-ci.yml добавил этап запуска написанного скрипта

![runner_install_curl](/src/images/deploy.png)

## Part 6. Дополнительно. Уведомления

Настроил уведомления о успешном/неуспешном выполнении пайплайна через бота с именем "[ваш nickname] DO6 CI/CD" в Telegram.

Для создания чат-бота в Telegram используется бот @BotFather. Чтобы создать своего бота, нужно найти @BotFather у себя в Telegram и перейти в чат с ним. Нажмите START. Чтобы создать нового бота, выберете в меню /newbot, введите имя бота в формате “имя_bot». После создания бота, вы получите API токен бота, который нужен будет в дальнейшей настройке.

Получить API токен бота телеграмТеперь нужно узнать user_id — это ID часа с ботом, в который он будет отправлять уведомления. Перейдите в ваш с ним чат и нажмите старт и отправить любой символ. После этого в браузере перейдите по адресу:

https://api.telegram.org/bot<token>/getUpdates — где <token> это API который выдал BotFather.

user_id бота telegramНа скриншоте выделен ваш ID. Итак, мы создали бота в Telegram и получили авторизационные данные. С помощью ранее созданного бота и полученных ID можно отсылать с сервера уведомления в Telegram чат, и таким образом получать какие-то интересные нам данные или алерты. Для отправки сообщения в чат, нужно использовать следующий URL:

https://api.telegram.org/bot<token>/sendMessage?chat_id=<chat_id>&text=<text>

Где:
<token> — это API который выдал @BotFather
<chat_id> — это ID вашего чата с ботом.
Например:

- curl -s -X POST https://api.telegram.org/bot944496485:AAEtGaGCVrQ7d26Rc3r_cqXPIhrKVokh8e4/sendMessage -d chat_id=336116180 -d text="Доброе утро, страна"

![runner_install_curl](/src/images/telega.png)
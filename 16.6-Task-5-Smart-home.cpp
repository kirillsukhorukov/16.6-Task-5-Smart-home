#include <iostream>
#include <sstream>

using namespace std;

enum switches
{
    home_power = 1,
    sockets = 2,
    ligths_inside = 4,
    ligths_outside = 8,
    heaters = 16,
    water_pipe_heating = 32,
    conditioner = 64
};

//Функция опроса пользователя
void user_survey(int & outside_temperature, int & inside_temperature, bool & motion_sensor, bool & lights_home, bool& state)
{
    //Флаг ошибки
    bool error;

    //Цикл опроса состояния датчиков
    do
    {
        //Пороги минимальной и максимальной температуры
        int min_temperature = -61;
        int max_temperature = 61;

        //Сброс флага ошибки
        error = false;

        //Буферная строка для ввода
        string buffer;

        //Запрос данных
        cout << endl << "Enter outside temperature ('C),  inside temperature ('C), " << endl
            << "is there any movement outside(yes/no), is the light on in the house (on/off): ";
        getline(cin, buffer);

        if (buffer == "state")
        {
            state = true;
            return;
        }

        //Разделение строки на составляющие
        stringstream bufferStream(buffer);
        int outside;
        int inside;
        string motion;
        string lights;
        bufferStream >> outside >> inside >> motion >> lights;

        //Проверка введенных данных
        if (outside > min_temperature && outside < max_temperature) outside_temperature = outside;
        else
        {
            cerr << "Error entering outside temperature! Re-enter." << endl;
            error = true;
        }

        if (inside > min_temperature && inside < max_temperature) inside_temperature = inside;
        else
        {
            cerr << "Error entering inside temperature! Re-enter." << endl;
            error = true;
        }

        if (motion == "yes") motion_sensor = true;
        else if (motion == "no") motion_sensor = false;
        else
        {
            cerr << "Error entering the presence of movement outside! Re-enter." << endl;
            error = true;
        }

        if (lights == "on") lights_home = true;
        else if (lights == "off") lights_home = false;
        else
        {
            cerr << "Error entering the presence of light in the house! Re-enter." << endl;
            error = true;
        }
    } while (error);
}

//Функция отображения состояния выключателей
void state_control(const int& switches_state, bool& state)
{
    //Сброс флага запроса статуса
    state = false;

    //Вывод сообщения о положении выключателей
    cout << endl << "*** STATE OF THE SWITCHES ***" << endl << endl;
    
    if (switches_state & home_power) cout << "Home power - ON" << endl;
    else cout << "Home power - OFF" << endl;

    if (switches_state & sockets) cout << "Sockets - ON" << endl;
    else cout << "Sockets - OFF" << endl;

    if (switches_state & ligths_inside) cout << "Lighting in the house - ON" << endl;
    else cout << "Lighting in the house - OFF" << endl;

    if (switches_state & ligths_outside) cout << "Garden lighting - ON" << endl;
    else cout << "Garden lighting - OFF" << endl;

    if (switches_state & heaters) cout << "Heaters - ON" << endl;
    else cout << "Heaters - OFF" << endl;

    if (switches_state & water_pipe_heating) cout << "Water pipe heating - ON" << endl;
    else cout << "Water pipe heating - OFF" << endl;

    if (switches_state & conditioner) cout << "Conditioner - ON" << endl;
    else cout << "Conditioner - OFF" << endl;

    cout << endl;
}

//Функция контроля обогрева водопровода
void water_pipe_heating_control(int & switches_state, const int & outside_temperature)
{
    int temp_state = switches_state;
    if (outside_temperature < 0)
    {
        switches_state |= water_pipe_heating;
        if (switches_state != temp_state) cout << "*** Water pipe heating - ON ***" << endl;
    }

    if (outside_temperature > 5)
    {
        switches_state &= ~water_pipe_heating;
        if (switches_state != temp_state) cout << "*** Water pipe heating - OFF ***" << endl;
    }
}

//Функция контроля садового освещения
void lights_outside_control(int & switches_state, const int & hour, const bool & motion_sensor)
{
    int temp_state = switches_state;
    if ((hour > 16 || hour <5) && motion_sensor)
    {
        switches_state |= ligths_outside;
        if (switches_state != temp_state) cout << "*** Garden lighting - ON ***" << endl;
    }

    if ((hour>4 && hour <17) || !motion_sensor)
    {
        switches_state &= ~ligths_outside;
        if (switches_state != temp_state) cout << "*** Garden lighting - OFF ***" << endl;
    }
}

//Функция контроля отопления
void heaters_control(int& switches_state, const int& inside_temperature)
{
    int temp_state = switches_state;
    if (inside_temperature < 22)
    {
        switches_state |= heaters;
        if (switches_state != temp_state) cout << "*** Heaters - ON ***" << endl;
    }

    if (inside_temperature > 24)
    {
        switches_state &= ~heaters;
        if (switches_state != temp_state) cout << "*** Heaters - OFF ***" << endl;
    }
}

//Функция контроля кондиционера
void conditioner_control(int& switches_state, const int& inside_temperature)
{
    int temp_state = switches_state;
    if (inside_temperature > 29)
    {
        switches_state |= conditioner;
        if (switches_state != temp_state) cout << "*** Conditioner - ON ***" << endl;
    }

    if (inside_temperature < 26)
    {
        switches_state &= ~conditioner;
        if (switches_state != temp_state) cout << "*** Conditioner - OFF ***" << endl;
    }
}

//Функция контроля внутреннего освещения
void lights_inside_control(int& switches_state, const int& hour, const int (& color_temperature)[24], const bool& lights_home)
{
    int temp_state = switches_state;
    if (lights_home)
    {
        switches_state |= ligths_inside;
        if (switches_state != temp_state) cout << "*** Lighting in the house -  ON ***" << endl;
        cout << "*** Color temperature - " << color_temperature[hour] << "K ***" << endl;
    }
    else
    {
        switches_state &= ~ligths_inside;
        if (switches_state != temp_state) cout << "*** Lighting in the house -  OFF ***" << endl;
    }
}

int main()

{
    cout << "-----Hello, it's smart home program-----!" << endl;
    cout << endl << "* Enter 'state' to display the state of the switches." << endl;
    
    //Инициализация переменной состояния выключателей
    //Включено питание всего дома и основные розетки
    int switches_state = 3;
    
    //Инициализация количества дней симуляции программы
    int simulation_days = 2;
    
    //Инициализация переменных, хранящих значения показаний датчиков
    int outside_temperature = 0;
    int inside_temperature = 22;
    bool motion_sensor = false;
    bool lights_home = false;
    
    //Инициализация флага вызова запроса о состоянии выключателей 
    bool state = false;
    
    //Массив значений цветовой температуры в течении суток
    int color_temperature[] = {5000, 5000, 5000, 5000, 5000, 5000,
                               5000, 5000, 5000, 5000, 5000, 5000,
                               5000, 5000, 5000, 5000, 5000, 4500,
                               4000, 3500, 2700, 2700, 2700, 2700};
    
    //Организация цикла работы программы
    for (int day = 0; day < simulation_days; day++)
        for (int hour = 0; hour < 24; hour++)
        {
            std::cout << endl << "Current time: " << hour << ":00" << endl;

            //Опрос пользователя
            user_survey(outside_temperature, inside_temperature, motion_sensor, lights_home, state);
            //cout << outside_temperature << " " << inside_temperature << " " << motion_sensor << " " << lights_home << endl;
            
            //Проверка вызова состояния выключателей
            while (state)
            {
                state_control(switches_state, state);
                user_survey(outside_temperature, inside_temperature, motion_sensor, lights_home, state);
            }
            
            //Обработка введенной пользователем информации
            water_pipe_heating_control(switches_state, outside_temperature);
            lights_outside_control(switches_state, hour, motion_sensor);
            heaters_control(switches_state, inside_temperature);
            conditioner_control(switches_state, inside_temperature);
            lights_inside_control(switches_state, hour, color_temperature, lights_home);
        }

}
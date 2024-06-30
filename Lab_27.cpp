#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include <mutex>

using namespace std;

const double PI = 3.14159265358979323846;
mutex mtx;  // м'ютекс для синхронізації виводу

// Функція для студентів, які рухаються хаотично
void chaotic_student(int id, double V, int N) {
    default_random_engine generator(random_device{}());
    uniform_real_distribution<double> direction_distribution(0.0, 2 * PI);

    double x = 0, y = 0;

    while (true) {
        double direction = direction_distribution(generator);
        double dx = V * cos(direction);
        double dy = V * sin(direction);

        x += dx;
        y += dy;

        // Використання м'ютекса для захисту виводу
        {
            lock_guard<mutex> lock(mtx);
            cout << "Chaotic Student " << id << " Position: (" << x << ", " << y << ")" << endl;
        }

        this_thread::sleep_for(chrono::seconds(N));
    }
}

// Функція для студенток, які рухаються по колу
void circular_student(int id, double V, double R) {
    double angle = 0;
    double x = R * cos(angle);
    double y = R * sin(angle);

    while (true) {
        x = R * cos(angle);
        y = R * sin(angle);

        // Використання м'ютекса для захисту виводу
        {
            lock_guard<mutex> lock(mtx);
            cout << "Circular Student " << id << " Position: (" << x << ", " << y << ")" << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(static_cast<int>((R * 2 * PI / V) * 1000 / 360)));

        angle += (V / R);
        if (angle > 2 * PI) {
            angle -= 2 * PI;
        }
    }
}

int main() {
    int num_chaotic_students = 3;  // Кількість студентів
    int num_circular_students = 3; // Кількість студенток
    double V = 0.1;  // Швидкість
    int N = 3;  // Інтервал зміни напрямку в секундах для студентів
    double R = 15.0;  // Радіус кола для студенток

    vector<thread> threads;

    for (int i = 0; i < num_chaotic_students; ++i) {
        threads.push_back(thread(chaotic_student, i, V, N));
    }

    for (int i = 0; i < num_circular_students; ++i) {
        threads.push_back(thread(circular_student, i, V, R));
    }

    for (auto& th : threads) {
        th.join();
    }

    return 0;
}

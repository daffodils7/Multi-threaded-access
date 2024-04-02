#include <iostream>
#include <vector>
#include <random>
#include <shared_mutex>
#include <thread>
using namespace std;
const int N = 100000;
int M = 2; // Number of concurrent workers

std::vector<int> S(N);
std::vector<std::shared_mutex> locks(N);
bool printflag = false;
int generateRandomNumber(int lower, int upper) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(lower, upper);
	return dis(gen);
}

void updateS(int i, int j) {
	int i_1 = (i + 1) % N;
	int i_2 = (i + 2) % N;

	std::shared_lock<std::shared_mutex> lock_i(locks[i]);
	std::shared_lock<std::shared_mutex> lock_i_1(locks[i_1]);
	std::shared_lock<std::shared_mutex> lock_i_2(locks[i_2]);
	std::unique_lock<std::shared_mutex> lock_j(locks[j], std::defer_lock);

	// Critical section
	lock_j.lock();
	S[j] = S[i] + S[i_1] + S[i_2];
	// End of critical section
	//不需要解锁，变量生命周期结束自然解锁
}

void worker(int workerId) {
	for (int k = 0; k < 10000; ++k) {
		int i = generateRandomNumber(0, N - 1);
		int j = generateRandomNumber(0, N - 1);

		updateS(i, j);
		if(printflag)cout <<  " update s[" << j << "]=" << S[j] << endl;
	}
}

int main() {
	// Initialize S with random values
	cout << "hello,The default value for the S array is between 0-100. If customization is required, please enter'x y',otherwise press enter." << endl;
	char x;
	int y;
	x = getchar();
	if (x != 10) {
		getchar();
		cin >> y;
		for (int i = 0; i < N; ++i) {
			S[i] = generateRandomNumber((int)(x-'0'), y);
		}
	}
	else {
		for (int i = 0; i < N; ++i) {
			S[i] = generateRandomNumber(0, 100);
		}
	}

	// Create worker threads
	std::vector<std::thread> threads;
	cout << "Please input M" << endl;
	cin >> M;
	getchar();
	cout << "The operation is about to begin. Do you want to know the update process(The output process may be chaotic due to thread parallelism)? Y/N" << endl;
	char flag = getchar();
	printflag = flag == 'Y' ? true : false;
	for (int i = 0; i < M; ++i) {
		threads.emplace_back(worker, i);
	}
	
	// Join worker threads
	for (auto& thread : threads) {
		thread.join();
	}
	cout << "Finish！" << endl;
	return 0;
}

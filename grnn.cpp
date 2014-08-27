#include <cerrno>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <approx.h>
#include <Eigen/Core>
using namespace Eigen;
using namespace std;

#define DAYS_TO_REMEMBER 14
#define FEATURES_PER_DAY 5

#define FEATURES_PER_INPUT DAYS_TO_REMEMBER * FEATURES_PER_DAY
#define BAND_WIDTH 1

typedef Matrix<float, 1, FEATURES_PER_DAY, RowMajor> Day;
typedef Matrix<float, DAYS_TO_REMEMBER, FEATURES_PER_DAY, RowMajor> InputBuffer;
typedef Matrix<float, DAYS_TO_REMEMBER, 1> ResultBuffer;
typedef Matrix<float, 1, FEATURES_PER_INPUT, RowMajor> Input;
Matrix<float, Dynamic, FEATURES_PER_INPUT + 1, RowMajor> trainingExamples;

void readIn(FILE *inFile, Day &day) {
	int count = fscanf(inFile, "%*d,0,%f,%f,%f,%f,%f\n", &day(0, 0), &day(0, 1), &day(0, 2), &day(0, 3), &day(0, 4));
	if (count == EOF || count != FEATURES_PER_DAY) {
		throw 0;
	}
}

Day readNext(FILE *inFile, InputBuffer &inBuf, ResultBuffer &resBuf, Day &nextIn, int &bufIdx, int bufLen) {
	Day dropped(inBuf.row(bufIdx));
	inBuf.row(bufIdx) = nextIn;
	readIn(inFile, nextIn);
	resBuf[bufIdx] = nextIn[3] - inBuf.row(bufIdx)[3];
	++bufIdx %= bufLen;
	return dropped;
}

float gaussian(float norm2, float band) {
	return fasterexp(-norm2 * band);
}

int main() {
	// Get current directory TODO: broken
	// char pathBuf[1024];
	// ssize_t len = readlink("/proc/self/exe", pathBuf, sizeof(pathBuf) - 1);
	// if (len == -1) return 1;// TODO exception
	// pathBuf[len] = '\0';
	// DIR *dir = opendir(pathBuf);// TODO: must free
	DIR *dir = opendir("data");
	cout << "flag1\n";
	// Loop over all data files in current directory
	errno = 0;
	dirent *file = readdir(dir);
	cout << "flag2\n";
	while (file) {
		char *fileName = file->d_name;
		// Get file extension
		const char *dot = strrchr(fileName, '.');
		// If file extension is .csv
		if (!dot || dot == fileName || strcmp(dot+1, "csv")) {
			file = readdir(dir);
			continue;
		}
		// Get stream to input file
		errno = 0;
		// TODO: optimize
		string filePath = "data/" + string(fileName);
		FILE *inFile = fopen(filePath.c_str(), "r");
		if (inFile == NULL) {
			cout << "errno: " << errno << endl;
		}
		cout << "flag4 " << filePath << endl;
		// Training examples are represented by circular buffers
		InputBuffer inBuf;
		ResultBuffer resBuf;
		int bufIdx = 0;
		// extra input struct to account for 1-day lag in results
		Day nextIn = Day();
		// Read 1st entry
		try {
			readIn(inFile, nextIn);
		}
		catch (int e) {
			fclose(inFile);
			file = readdir(dir);
			continue;
		}
		cout << "flag5 " << filePath << endl;
		// Fill input buffer
		for (int d = 0; d < DAYS_TO_REMEMBER; d++) {
			try {
				readNext(inFile, inBuf, resBuf, nextIn, bufIdx, DAYS_TO_REMEMBER);
			}
			catch (int e) {
				fclose(inFile);
				file = readdir(dir);
				continue;
			}
		}
		// Initialization for feature normalization
		Day sum = inBuf.colwise().sum();
		Day sum2 = inBuf.cwiseProduct(inBuf).colwise().sum();
		while(true) {
			// Convert raw data into useful info
			Input in = Input();
			// Recover contiguous input from circular buffer
			in << inBuf.block<Dynamic, FEATURES_PER_DAY>(bufIdx, 0, DAYS_TO_REMEMBER - bufIdx, FEATURES_PER_DAY);
			in << inBuf.block<Dynamic, FEATURES_PER_DAY>(0, 0, bufIdx, FEATURES_PER_DAY);
			// Feature normalization
			Day mean = sum / DAYS_TO_REMEMBER;
			Day stdDev = (sum2 * DAYS_TO_REMEMBER - sum.cwiseProduct(sum)).cwiseSqrt() / DAYS_TO_REMEMBER;
			//in 
			// Add training example
			// TODO
			// Get next input
			Day droppedDay;
			try {
				droppedDay = readNext(inFile, inBuf, resBuf, nextIn, bufIdx, DAYS_TO_REMEMBER);
			}
			catch (int e) {
				break;
			}
			// Update feature normalization
			sum -= droppedDay;
			sum2 -= droppedDay.cwiseProduct(droppedDay);
			Day newDay = inBuf.row(bufIdx);
			sum += newDay;
			sum2 += newDay.cwiseProduct(newDay);
		}
		fclose(inFile);
		file = readdir(dir);
		break;// TODO: temp
	}
	if (errno) {
		// TODO: exception
	}
	closedir(dir);
	return 0;
}
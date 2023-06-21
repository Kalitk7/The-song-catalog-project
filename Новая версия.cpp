#define __CRT_SECURE_NO_WARNINGS 

#include <iostream> 
#include <windows.h> 
#include <string> 
#include <fstream> 

using namespace std;
HANDLE Consolechka = GetStdHandle(STD_OUTPUT_HANDLE);


struct Song
{
    string author; // автор
    string title; // название песни
    int year = -1; // год издания 
    string text; // текст песни
    string fileWithText; // файл с текстом
}; 


void IdentifySize(int& size);
Song CreateSong(int& size);
Song TakingSongFromFile(int num);
void ShowDialogToAddText(Song& structure_object);
void SaveTextToFile(Song& structure_object);
void AddTextFromFile(Song& structure_object);
void InitializingFileWithText(Song& structure_object);
void SaveSize(int& size);
void IdentifyCatalog();
void SaveCatalogToFile(Song& structure_object, int size);
void PrintSongList();
void PrintTextOnScreen(string& songName);
void ReplaceTextInFile(string& fileName, string& textToReplace, string& replacementText);
void MenuForWorkingWithText();
void DeleteTextSong(string& songName);
void DeleteSong();
void SearchSong();
void SearchByAuthor();
void GoToXY(short x, short y);
void ConsoleCursorVisible(bool show, short size);


int main(){
	// русская кодировка
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);


	// кастомизация консоли
	MoveWindow(GetConsoleWindow(), 480, 100, 500, 700, true);
	SetConsoleTextAttribute(Consolechka, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	system("CLS");
	ConsoleCursorVisible(false, 10);
	SetConsoleTitle(L"🎶 КАТАЛОГ ТЕКСТОВ ПЕСЕН 🎶");


	//начало программы
	int menu = 0;
	int size = 0;

	IdentifySize(size);
	IdentifyCatalog();


	cout << R"(  __| |____________________________________________| |__
 (__   ____________________________________________   __)
    | |           -----Главное меню----            | |
    | |                                            | |
    | |      1 - Показ всех песен                  | |
    | |      2 - Добавление песни                  | |
    | |      3 - Поиск по названию                 | |
    | |      4 - Поиск по автору                   | |
    | |      5 - Изменение текста песни            | |
    | |      6 - Удаление песни                    | |
    | |      0 - Выход                             | |
    | |                                            | |
    | |                                            | |
    | |                                  (\___ /)  | |
    | |                                  (= '.' =) | |
    | |                                  (")_(") ) | |
  __| |____________________________________________| |__
 (__   ____________________________________________   __)
    | |                                            | |)" << endl;
	do {
		cin >> menu;
		cin.ignore();

		switch (menu) {
		case 1:
			PrintSongList();
			break;

		case 2:
			CreateSong(size);
			break;

		case 3:
			SearchSong();
			break;

		case 4:
			SearchByAuthor();
			break;

		case 5:
			MenuForWorkingWithText();
			break;

		case 6:
			DeleteSong();
			break;
		case 0:
			break;
		}
	} while (menu < 0 || menu > 6);

	SaveSize(size);

	cout << endl << "\t\t-------ОКОНЧАНИЕ РАБОТЫ-------" << endl;
}


void IdentifySize(int& size) {
	string path = "developer_folder\\size.txt";

	ifstream forOpen, fill(path);
	ofstream forCreate;

	forOpen.open(path);

	if (!forOpen.is_open())
	{
		forCreate.open(path);
		forCreate.close();
	}

	forOpen.open(path);

	fill >> size;

	forOpen.close();
	fill.close();
}


void IdentifyCatalog() { //создает при первом запуске файл, проверяет факт его существования
	string path = "developer_folder\\song_list.txt";
	ifstream forOpen(path);
	ofstream forCreate;

	forOpen.open(path);

	if (!forOpen.is_open())
	{
		forCreate.open(path);
		forCreate.close();
	}
}


Song CreateSong(int& size) {
	Song finch; // !!зяблик!!

	cout << "Автор: ";
	cin >> finch.author;

	cout << "Название: ";
	cin >> finch.title;

	cout << "Год создания: ";
	cin >> finch.year;

	ShowDialogToAddText(finch);
	InitializingFileWithText(finch);
	size++;
	SaveCatalogToFile(finch, size);
	return finch;
}


void ShowDialogToAddText(Song& structure_object) {
	int choise = 0;


	cout << R"(   __| |____________________________________________| |__
  (__   ____________________________________________   __)
    | |          -----Текст песни----              | |
    | |                                            | |
    | |                                            | |
    | |        1 - ввести с клавиатуры             | |
    | |        2 - ввести из файла                 | |
    | |                                            | |
    | |                                            | |
  __| |____________________________________________| |__
 (__   ____________________________________________   __)
    | |                                            | |)" << endl;

	do {
		cin >> choise;
		cin.ignore();
	} while (choise < 1 || choise > 2);

	if (choise == 1) SaveTextToFile(structure_object);
	else AddTextFromFile(structure_object);

}


void SaveTextToFile(Song& structure_object) {
	string path = "developer_folder\\catalog\\" + structure_object.title + ".txt";
	ofstream save;

	save.open(path, ofstream::out);

	if (!save.is_open()) cout << "Ошибка! SaveTextToFile";
	else
	{
		int pos = -1;
		cout << "Для окончания ввода введите %" << endl;

		do
		{
			string temp, end = "%";
			getline(cin, temp);
			save << temp << endl;
			pos = temp.rfind(end);
		} while (pos == -1);

		cout << endl << "Текст сохранён в файл " << path << endl;
	}

	structure_object.fileWithText = path;
	save.close();
}


void AddTextFromFile(Song& structure_object) {
	string pathSourse, pathDest;

	cout << "Введите имя файла без расширения, чтобы загрузить текст: ";
	getline(cin, pathSourse);

	pathSourse = "user_folder\\" + pathSourse + ".txt";
	pathDest = "developer_folder\\catalog\\" + structure_object.title + ".txt";

	ifstream sourseFile;
	ofstream destFile;

	sourseFile.open(pathSourse);
	destFile.open(pathDest, ofstream::out);

	if (!sourseFile.is_open()) cout << "Ошибка открытия файла! AddTextFromFile" << endl;
	else {
		string str;
		getline(sourseFile, str);
		while (!sourseFile.eof()) {
			destFile << str << endl;
			getline(sourseFile, str);
		}
		destFile << str;
		cout << "Текст из файла " << pathSourse << " добавлен." << endl;
	}

	structure_object.fileWithText = pathDest;
	sourseFile.close();
	destFile.close();
}


void InitializingFileWithText(Song& structure_object) { 
	string str, path = structure_object.fileWithText;
	ifstream File;

	File.open(path);
	getline(File, str);
	structure_object.text = str;
	File.close();
}


void SaveSize(int& size) {
	string path = "developer_folder\\size.txt";
	ofstream fileOpen;

	fileOpen.open(path, ofstream::out); 
	fileOpen << size;
	fileOpen.close();
}


void SaveCatalogToFile(Song& structure_object, int size) {
	string path = "developer_folder\\song_list.txt";
	ofstream fileOpen;

	fileOpen.open(path, ofstream::app);

	fileOpen << "\t\tПесня номер " << size << "\n" <<
	"Автор: " << structure_object.author << "\n" << 
	"Название: " << structure_object.title << "\n" << 
	"Год: " << structure_object.year << "\n" <<
	"Начало текста: " << structure_object.text << "\n" << 
	"Файл с текстом: " << structure_object.fileWithText << "\n" << endl;

	cout << "Каталог сохранен в файл " << path << endl;

	fileOpen.close();
}


void PrintSongList(){
	string line;
	ifstream file("developer_folder\\song_list.txt");

	if (file.peek() == ifstream::traits_type::eof()) {
		cout << "Песни отсутствуют!" << endl;
		return;
	}

	while (getline(file, line))  cout << line << endl;

	file.close();
}


void MenuForWorkingWithText() {
	string textToReplace, replacementText, file;
	string songName;
	int choise = 0, num = -1;

	Song buffer;

		cout << R"(  __| |____________________________________________| |__
 (__   ____________________________________________   __)
    | |        --Меню работы с текстом--           | |
    | |                                            | |
    | |                                            | |
    | |      1 - Перезапись текста песни           | |
    | |      2 - Текст на экран                    | |
    | |      3 - Удаление текста                   | |
    | |                                            | |
  __| |____________________________________________| |__
 (__   ____________________________________________   __)
    | |                                            | |)" << endl;

	do{
		cin >> choise;
		cin.ignore();
	} while (choise < 0 || choise > 4);

	cout << "СПИСОК ПЕСЕН: " << endl;
	PrintSongList();

	switch (choise)
	{
		
	case 1:		
		cout << "\nВВЕДИТЕ НОМЕР ПЕСНИ ДЛЯ ПЕРЕЗАПИСИ ЕЁ ТЕКСТА: ";
		cin >> num;
		cin.ignore();

		buffer = TakingSongFromFile(num);
		textToReplace = buffer.text;

		ShowDialogToAddText(buffer);
		InitializingFileWithText(buffer);

		replacementText = buffer.text;
		file = "developer_folder\\song_list.txt";

		ReplaceTextInFile(file, textToReplace, replacementText);
		break;
	case 2:
		cout << "\nВВЕДИТЕ НОМЕР ПЕСНИ ДЛЯ ЕЁ ВЫВОДА НА ЭКРАН: ";
		cin >> num;
		cin.ignore();

		buffer = TakingSongFromFile(num);
		songName = buffer.title;

		PrintTextOnScreen(songName);
		break;
	case 3:
		cout << "\nВВЕДИТЕ НОМЕР ПЕСНИ ДЛЯ УДАЛЕНИЯ ЕЁ ТЕКСТА: ";
		cin >> num;
		cin.ignore();

		buffer = TakingSongFromFile(num);
		textToReplace = buffer.text;
		songName = buffer.title;
		DeleteTextSong(songName);
		replacementText = "Текст был удален!";
		file = "developer_folder\\song_list.txt";

		ReplaceTextInFile(file, textToReplace, replacementText);
		break;
	case 0:
		break;
	}
}


void PrintTextOnScreen(string& songName) {
	string line, path = "developer_folder\\catalog\\" + songName + ".txt";
	ifstream file(path);

	if (!file.is_open()) {
		cout << "Введенная песня не существует!" << endl;
		return;
	}

	while (getline(file, line)) cout << line << endl;

	file.close();
}


Song TakingSongFromFile(int num) {
	Song gate; //!!калитка!!

	string line, path = "developer_folder\\song_list.txt";
	ifstream file(path); 


	if (!file.is_open()) {
		cout << "Ошибка открытия файла " << path << endl;
		return gate;
	}

	// Пропуск нужного количество песен и строк, выглядит адски
	for (int i = 1; i < num; i++) for (int j = 0; j < 7; j++) getline(file, line);


	getline(file, line); // Пропуск "Песня номер X"
	getline(file, line); 
	gate.author = line.substr(7);
	getline(file, line); 
	gate.title = line.substr(10);
	getline(file, line); 
	gate.year = stoi(line.substr(5));
	getline(file, line); 
	gate.text = line.substr(15); 
	getline(file, line); 
	gate.fileWithText = line.substr(16);

	file.close();

	return gate;
}


void ReplaceTextInFile(string& fileName, string& textToReplace, string& replacementText) {
	string line;
	ifstream inputFile(fileName);
	ofstream tempFile("developer_folder\\temporary.txt");
	ofstream forCreate;


	if (!inputFile.is_open()) {
		cout << "Ошибка открытия song catalog! ReplaceTextInFile" << endl;
		return;
	}

	
	if (!tempFile.is_open()) {
		cout << "Ошибка создания копии! ReplaceTextInFile" << endl;
		return;
	}

	while (getline(inputFile, line)) {
		size_t pos = line.find(textToReplace); // замена нужной части текста 

		if (pos != string::npos) line.replace(pos, textToReplace.length(), replacementText);
		tempFile << line << std::endl; // запись обновленной строки
	}

	inputFile.close();
	tempFile.close();

	remove(fileName.c_str()); // удаление исходника
	rename("developer_folder\\temporary.txt", fileName.c_str()); // переименовние нового файла. 

}


void DeleteTextSong(string& songName){
	string path = "developer_folder\\catalog\\" + songName + ".txt";
	ofstream file(path, ios::trunc);
	file.close();
}


void DeleteSong() {
	int num;
	string file = "developer_folder\\song_list.txt";

	PrintSongList();

	cout << "\nВВЕДИТЕ НОМЕР ПЕСНИ ДЛЯ ЕЁ УДАЛЕНИЯ: ";
	cin >> num;
	cin.ignore();

	Song buffer = TakingSongFromFile(num);

	string textToReplace = "Автор: " + buffer.author; 
	string replacementText = "Данная"; 
	ReplaceTextInFile(file, textToReplace, replacementText);

	textToReplace = "Название: " + buffer.title;
	replacementText = "песня"; 
	ReplaceTextInFile(file, textToReplace, replacementText);

	textToReplace = "Год: " + to_string(buffer.year); 
	replacementText = "была"; 
	ReplaceTextInFile(file, textToReplace, replacementText);

	textToReplace = "Начало текста: " + buffer.text;
	replacementText = "всецело"; 
	ReplaceTextInFile(file, textToReplace, replacementText);

	textToReplace = "Файл с текстом: " + buffer.fileWithText;
	replacementText = "удалена:((";
	ReplaceTextInFile(file, textToReplace, replacementText);


	string filename = "developer_folder\\catalog\\" + buffer.title + ".txt";

	if (remove(filename.c_str()) == 0) cout << "Файл с текстом успешно удален" << endl;
	else cout << "Ошибка при удалении файла с текстом" << endl;
}


void SearchSong() {
	string songName;
	cout << "Введите название песни: ";
	getline(cin, songName);

	ifstream file("developer_folder\\song_list.txt");
	string line;
	bool songFound = false;
	int songCount = 0;

	while (getline(file, line)) {
		if (line.find("Название: " + songName) != string::npos) {
			songFound = true;
			break;
		}
		if (line.empty()) {
			songCount++;
		}
	}

	file.close();

	if (songFound) cout << "Песня \"" << songName << "\" найдена. Порядковый номер: " << (songCount + 1) << endl;
	else cout << "Песня \"" << songName << "\" не найдена." << endl;
}


void SearchByAuthor(){
	string line;
	bool found = false;
	string author;
	ifstream file("developer_folder\\song_list.txt");

	cout << "Введите автора: ";
	getline(cin, author);

	if (file.fail())
	{
		cout << "Ошибка открытия файла song_list.txt!" << endl;
		return;
	}

	cout << "Он известен следующими песнями:" << endl;

	while (getline(file, line))
	{
		if (line.find("Автор: " + author) != string::npos)
		{
			found = true;
			cout << line << endl;

			for (int i = 0; i < 4; ++i)
			{
				getline(file, line);
				cout << line << endl;
			}
			cout << endl;
		}
	}

	file.close();

	if (!found)cout << "Песни данного автора не найдены. ноунейм(" << endl;
}


//ДЛЯ КОНСОЛИ


void GoToXY(short x, short y) {
	SetConsoleCursorPosition(Consolechka, {x, y});
}


void ConsoleCursorVisible(bool show, short size) {
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(Consolechka, &structCursorInfo);

	structCursorInfo.bVisible = show; // видимость курсора
	structCursorInfo.dwSize = size; // размер курсора

	SetConsoleCursorInfo(Consolechka, &structCursorInfo);
}
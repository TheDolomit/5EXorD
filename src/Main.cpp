#include "Encryption.h"
#include "Decryption.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
	#define clear_console() system("cls")
#else
	#define clear_console() system("clear")
#endif

int main()
{
	std::cout << "Hello! Welcome to EXorD! Would you like to (E)ncrypt, (D)ecrypt, Encrypt a file(EF) or Decrypt a file(DF)?: ";

	std::string choice;

	std::getline(std::cin, choice);

	if (choice == "E" || choice == "e")
	{
		clear_console();

		std::cout << "Please enter the text to encrypt: ";
		std::getline(std::cin, choice);

		std::string input = choice;

		clear_console();

		std::cout << "Please supply a seed: ";
		std::getline(std::cin, choice);

		size_t seed = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply a modifier: ";
		std::getline(std::cin, choice);

		size_t modifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply an amplifier: ";
		std::getline(std::cin, choice);

		size_t amplifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Here is your encrypted string:\n" << EXorD::Encrypt(input, seed, modifier, amplifier);

		std::cin.get();

		return 0;
	}
	else if (choice == "D" || choice == "d")
	{
		clear_console();

		std::cout << "Please enter the text to decrypt: ";
		std::getline(std::cin, choice);

		std::string input = choice;

		clear_console();

		std::cout << "Please supply a seed: ";
		std::getline(std::cin, choice);

		size_t seed = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply a modifier: ";
		std::getline(std::cin, choice);

		size_t modifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply an amplifier: ";
		std::getline(std::cin, choice);

		size_t amplifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Here is your decrypted string:\n" << EXorD::Decrypt(input, seed, modifier, amplifier);

		std::cin.get();

		return 0;
	}
	else if (choice == "EF" || choice == "ef")
	{
		clear_console();

		std::cout << "Please enter the path of the file to encrypt (accepts relative paths): ";
		std::getline(std::cin, choice);

		std::string filepath = choice;

		clear_console();

		std::ifstream stream(filepath);

		if (!stream)
		{
			std::cerr << "Unable to find file with filepath: '" << filepath << "'!\n";
			std::cin.get();
			return 0;
		}

		std::ostringstream contents;

		contents << stream.rdbuf();

		stream.close();

		std::cout << "Please supply a seed: ";
		std::getline(std::cin, choice);

		size_t seed = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply a modifier: ";
		std::getline(std::cin, choice);

		size_t modifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply an amplifier: ";
		std::getline(std::cin, choice);

		size_t amplifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::ofstream ofstream(filepath);

		ofstream << EXorD::Encrypt(contents.str(), seed, modifier, amplifier);

		std::cout << "Done.\n";

		std::cin.get();

		return 0;
	}
	else if (choice == "DF" || choice == "df")
	{
		clear_console();

		std::cout << "Please enter the path of the file to decrypt (accepts relative paths): ";
		std::getline(std::cin, choice);

		std::string filepath = choice;

		clear_console();

		std::ifstream stream(filepath);

		if (!stream)
		{
			std::cerr << "Unable to find file with filepath: '" << filepath << "'!\n";
			std::cin.get();
			return 0;
		}

		std::ostringstream contents;

		contents << stream.rdbuf();

		stream.close();

		std::cout << "Please supply a seed: ";
		std::getline(std::cin, choice);

		size_t seed = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply a modifier: ";
		std::getline(std::cin, choice);

		size_t modifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::cout << "Please supply an amplifier: ";
		std::getline(std::cin, choice);

		size_t amplifier = std::strtoull(choice.c_str(), nullptr, 0);

		clear_console();

		std::ofstream ofstream(filepath);

		ofstream << EXorD::Decrypt(contents.str(), seed, modifier, amplifier);

		std::cout << "Done.\n";

		std::cin.get();

		return 0;
	}

	std::cin.get();
}
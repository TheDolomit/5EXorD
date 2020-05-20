#pragma once

#include <random>
#include <sstream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

namespace EXorD
{
	static std::string Encrypt(const std::string& input, uint64_t seed, uint64_t modifier, uint64_t amplifier)
	{
		if (input.empty())
			return "";

		// Generate a pseudo-random number that will be used throughout the encryption process
		std::mt19937_64 rng(std::random_device{}());
		const std::uniform_int_distribution<uint64_t> uid(0, UINT_MAX ^ (modifier * -1)); // If we do a basic unary '-' then we get an error
		auto gen_rand = [&rng, &uid] // Lambda that is capable of generating a random number between [0, infinity)
		{
			return uid(rng);
		};

		// Generate a random key that we will use throughout the encryption process
		uint64_t random = gen_rand();

		// Generate a key specific to this seed, modifier and amplifier combination
		std::string special = [&]
		{
			// Generate seven numbers that are guaranteed to be different for each SMA combination
			
			const size_t secret1 = seed ^ modifier ^ amplifier ^ random;
			const size_t secret2 = random ^ (((amplifier == 0) ? 1 : amplifier) * -1);
			const size_t secret3 = secret2 | secret1 + amplifier;
			const size_t secret4 = modifier ^ (secret3 + seed);
			const size_t secret5 = ((seed / modifier) & amplifier) ^ secret4;
			const size_t secret6 = ((seed / amplifier) + modifier) ^ secret5;
			rng.seed(secret5 + secret6);
			const size_t secret7 = gen_rand() ^ (secret5 + secret6);

			return std::to_string(secret7) + std::to_string(secret6) + std::to_string(secret5) + std::to_string(secret1);
		}();

		// Now that we have this sorted, we need to build our final string
		std::ostringstream result;

		std::string random_str = std::to_string(random); // A stringified version of our random number
		const size_t random_length = random_str.length(); // The length of our random string

		// Iterate through each digit of this random string and mangle it to retrieve a mangled pseudo-random string (MR)
		for (auto& c : random_str)
		{
			c = (c ^ random_length ^ 'J');
		}

		// Push the random string length and the random string into the result buffer
		result << (char)(random_str.length() + 'a') << random_str;
		
		std::vector<uint64_t> previous_xors; // Stores all the previously XOR'd numbers

		size_t string_pointer = 0;
		bool stop_round_loading = false;

		std::uniform_int_distribution<int> uidfrom_1_to_20(1, 20);
		std::uniform_int_distribution<int> uidfrom_1_to_5(1, 5);
		std::uniform_int_distribution<int> uidfrom_0_to_127(0, 127);

		size_t final_xor = uidfrom_0_to_127(rng);

		for (size_t i = 0; i < special.length(); i += 2)
			final_xor ^= special[i];

		// Perform XOR operations on all the characters of our input string
		for (char c : input)
		{
			if (special.length() % 2) // Number is NOT divisible by two
			{
				c ^= (int)(special.back()) - '0';
				special.pop_back(); // Make the length of the string a multiple of two
			}

			// If we are about to read too far into out string, simply revert back to the beginning
			if ((string_pointer * 2) == special.length())
			{
				stop_round_loading = true;
				string_pointer = 0;
			}

			const auto stringified_number = special.substr(string_pointer++ * 2, 2);

			// The numeric representation of our sector
			const auto number = std::atoi(stringified_number.c_str());

			// XOR operations

			c ^= (number + uidfrom_1_to_20(rng)) ^ '\n' ^ final_xor;
			
			for (auto&& prev : previous_xors)
			{
				c ^= prev;
			}

			if(!stop_round_loading)
				previous_xors.push_back(number);

			// Special cases:
			// To avoid things like newlines and invalid characters, we simply signify
			// To the decryptor that we modified the base value (with a prepended char)
			if (c < '!')
				result << (char)('+' + uidfrom_1_to_5(rng)) << (char)(c + '!');
			else if(c > 124)
				result << (char)('!' + uidfrom_1_to_5(rng)) << (char)(c - '!');
			else
				result << (char)('j' + uidfrom_1_to_5(rng)) << c;

		}

		return result.str();
	}

	// --------------------------------------------------------------------------------------------------------------

	static std::string EncryptFileContents(const std::filesystem::path& path, uint64_t seed, uint64_t modifier, uint64_t amplifier)
	{
		if (!std::filesystem::exists(path) || path.string().empty() || std::filesystem::is_empty(path))
			return "";

		// Read the file and extract its contents
		std::ifstream file(path, std::ios::binary);

		std::string data = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

		file.close();

		// Generate a pseudo-random number that will be used throughout the encryption process
		std::mt19937_64 rng(std::random_device{}());
		const std::uniform_int_distribution<uint64_t> uid(0, UINT_MAX ^ (modifier * -1)); // If we do a basic unary '-' then we get an error
		auto gen_rand = [&rng, &uid] // Lambda that is capable of generating a random number between [0, infinity)
		{
			return uid(rng);
		};

		// Generate a random key that we will use throughout the encryption process
		uint64_t random = gen_rand();

		// Generate a key specific to this seed, modifier and amplifier combination
		std::string special = [&]
		{
			// Generate seven numbers that are guaranteed to be different for each SMA combination

			const size_t secret1 = seed ^ modifier ^ amplifier ^ random;
			const size_t secret2 = random ^ (((amplifier == 0) ? 1 : amplifier) * -1);
			const size_t secret3 = secret2 | secret1 + amplifier;
			const size_t secret4 = modifier ^ (secret3 + seed);
			const size_t secret5 = ((seed / modifier) & amplifier) ^ secret4;
			const size_t secret6 = ((seed / amplifier) + modifier) ^ secret5;
			rng.seed(secret5 + secret6);
			const size_t secret7 = gen_rand() ^ (secret5 + secret6);

			return std::to_string(secret7) + std::to_string(secret6) + std::to_string(secret5) + std::to_string(secret1);
		}();

		// Now that we have this sorted, we need to build our final string
		std::ostringstream result;

		std::string random_str = std::to_string(random); // A stringified version of our random number
		const size_t random_length = random_str.length(); // The length of our random string

		// Iterate through each digit of this random string and mangle it to retrieve a mangled pseudo-random string (MR)
		for (auto& c : random_str)
		{
			c = (c ^ random_length ^ 'J');
		}

		// Push the random string length and the random string into the result buffer
		result << (char)(random_str.length() + 'a') << random_str;

		std::vector<uint64_t> previous_xors; // Stores all the previously XOR'd numbers

		size_t string_pointer = 0;
		bool stop_round_loading = false;

		std::uniform_int_distribution<int> uidfrom_1_to_20(1, 20);
		std::uniform_int_distribution<int> uidfrom_1_to_5(1, 5);
		std::uniform_int_distribution<int> uidfrom_0_to_127(0, 127);

		size_t final_xor = uidfrom_0_to_127(rng);

		for (size_t i = 0; i < special.length(); i += 2)
			final_xor ^= special[i];

		// Perform XOR operations on all the characters of our input string
		for (char c : data)
		{
			if (special.length() % 2) // Number is NOT divisible by two
			{
				c ^= (int)(special.back()) - '0';
				special.pop_back(); // Make the length of the string a multiple of two
			}

			// If we are about to read too far into out string, simply revert back to the beginning
			if ((string_pointer * 2) == special.length())
			{
				stop_round_loading = true;
				string_pointer = 0;
			}

			const auto stringified_number = special.substr(string_pointer++ * 2, 2);

			// The numeric representation of our sector
			const auto number = std::atoi(stringified_number.c_str());

			// XOR operations

			c ^= (number + uidfrom_1_to_20(rng)) ^ '\n' ^ final_xor;

			for (auto&& prev : previous_xors)
			{
				c ^= prev;
			}

			if (!stop_round_loading)
				previous_xors.push_back(number);

			// Special cases:
			// To avoid things like newlines and invalid characters, we simply signify
			// To the decryptor that we modified the base value (with a prepended char)

			if (c < '!')
				result << (char)('+' + uidfrom_1_to_5(rng)) << (char)(c + '!');
			else if (c > 122)
				result << (char)('!' + uidfrom_1_to_5(rng)) << (char)(c - '!');
			else
				result << (char)('j' + uidfrom_1_to_5(rng)) << c;
			
		}

		return result.str();
	}
};
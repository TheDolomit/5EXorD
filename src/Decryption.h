#pragma once

#include <string>
#include <random>
#include <sstream>

namespace EXorD
{
	static std::string Decrypt(const std::string& input, uint64_t seed, uint64_t modifier, uint64_t amplifier)
	{
		// To decrypt the string, we need to build up a correct special value

		// First, we extract the random value from our encrypted input
		const size_t random_length = (size_t)input.front() - 'a';

		std::string random_string = input.substr(1, random_length);

		// Demangle the random string
		for (char& c : random_string)
		{
			c ^= random_length ^ 'J';
		}

		uint64_t random = std::strtoull(random_string.c_str(), nullptr, 0);

		// Initialize pseudo-random number generators
		std::mt19937_64 rng;
		const std::uniform_int_distribution<uint64_t> uid(0, UINT_MAX ^ (modifier * -1)); // If we do a basic unary '-' then we get an error
		auto gen_rand = [&rng, &uid] // Lambda that is capable of generating a random number between [0, infinity)
		{
			return uid(rng);
		};

		// Rebuild our special string
		std::string special = [&]
		{
			const size_t secret1 = seed ^ modifier ^ amplifier ^ random;
			const size_t secret2 = random + (((amplifier == 0) ? 1 : amplifier) * -1);
			const size_t secret3 = secret2 & secret1;
			const size_t secret4 = modifier | (secret3 & seed);
			rng.seed(secret4);
			const size_t secret5 = gen_rand();

			return std::to_string(secret5) + std::to_string(secret4) + std::to_string(secret1);
		}();

		// Now that we have this sorted, we need to build our final string
		std::ostringstream result;

		std::vector<uint64_t> previous_xors; // Stores all the previously XOR'd numbers

		size_t string_pointer = 0;
		bool stop_round_loading = false;

		std::uniform_int_distribution<int> uidfrom_1_to_20(1, 20);
		std::uniform_int_distribution<int> uidfrom_1_to_5(1, 5);
		std::uniform_int_distribution<int> uidfrom_0_to_127(0, 127);

		// Create a final_xor value that will be the last round shot at our characters
		size_t final_xor = uidfrom_0_to_127(rng);

		for(size_t i = 0; i < special.length(); i += 2)
			final_xor ^= std::atoi(special.substr(i, 2).c_str());

		// Perform XOR operations on all the characters of our input string
		for (size_t i = 1 + random_length; i < input.length(); i += 2)
		{
			// Generate two random numbers, since we have control over rng
			uint64_t rand_between_1_20 = uidfrom_1_to_20(rng);
			uint64_t rand_between_1_5 = uidfrom_1_to_5(rng);

			char special_char = input[i];
			char actual_char = input[i + 1];

			if ((special_char - rand_between_1_5) == '+')
				actual_char -= '!';
			else if ((special_char - rand_between_1_5) == '!')
				actual_char += '!';

			// If we are about to read too far into out string, simply revert back to the beginning
			if ((string_pointer * 2) == special.length())
			{
				stop_round_loading = true;
				string_pointer = 0;
			}

			const auto stringified_number = special.substr(string_pointer++ * 2, 2);

			// The numeric representation of our sector
			const auto number = std::atoi(stringified_number.c_str());

			for (auto&& prev : previous_xors)
				actual_char ^= prev;

			actual_char ^= (number + rand_between_1_20) ^ '\n' ^ final_xor;

			if (special.length() % 2) // Number is NOT divisible by two
			{
				actual_char ^= (int)(special.back()) - '0';
				special.pop_back(); // Make the length of the string a multiple of two
			}

			if (!stop_round_loading)
				previous_xors.push_back(number);

			result << actual_char;
		}

		return result.str();
	}
}
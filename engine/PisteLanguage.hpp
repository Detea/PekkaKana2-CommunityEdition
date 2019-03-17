//#########################
//PisteEngine - PisteLanguage
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef PISTELANGUAGE
#define PISTELANGUAGE

#include "platform.hpp"
#include <string>

const int MAX_TEXTS = 200;
const int MAX_TEXT_LENGTH = 80;
const int MAX_HEAD_LENGTH = 50;

class PisteLanguage{
	public:
		PisteLanguage();
		PisteLanguage(std::string file);

		~PisteLanguage();
		bool Read_File(std::string filename);
		int Hae_Indeksi(char *title);
		char* Hae_Teksti(int index);
		void Korvaa_Teksti(int index, char *teksti);

	private:
		char tekstit[MAX_TEXTS][MAX_TEXT_LENGTH+1];
		char otsikot[MAX_TEXTS][MAX_HEAD_LENGTH+1];
		int  read;
};

#endif

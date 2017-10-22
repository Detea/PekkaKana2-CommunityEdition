//Sprite Movement

double	sprite_x,
		sprite_y,
		sprite_a,
		sprite_b,

		sprite_vasen,
		sprite_oikea,
		sprite_yla,
		sprite_ala;

int		sprite_leveys,
		sprite_korkeus;

int		kartta_vasen,
		kartta_yla,
		x = 0,
		y = 0;

bool	oikealle,
		vasemmalle,
		ylos,
		alas,

		vedessa;

BYTE   max_nopeus;

//Colision with the block sprite
void PK_Tutki_Seina2(PK2Sprite &sprite, PK2BLOCK &palikka){

	//left and right
	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla){
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea){
			// Tutkitaan onko sprite menossa oikeanpuoleisen palikan sis��n.
			if (sprite_oikea+sprite_a < palikka.oikea){
				// Onko palikka sein�?
				if (palikka.oikealle == BLOCK_SEINA){
					oikealle = false;
					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}

			if (sprite_vasen+sprite_a > palikka.vasen){
				if (palikka.vasemmalle == BLOCK_SEINA){
					vasemmalle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;

	//ceil and floor

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen){
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala){
			if (sprite_ala+sprite_b-1 <= palikka.ala){
				if (palikka.alas == BLOCK_SEINA){
					alas = false;

					if (palikka.koodi == BLOCK_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
						if (palikka.koodi != BLOCK_HISSI_HORI)
							sprite_y = palikka.yla - sprite_korkeus /2;
				}
			}

			if (sprite_yla+sprite_b > palikka.yla){
				if (palikka.ylos == BLOCK_SEINA){
					ylos = false;

					if (sprite_yla < palikka.ala)
						if (palikka.koodi != BLOCK_HISSI_HORI)
							sprite.kyykky = true;
				}
			}
		}
	}
}
//Examine blocks
void PK_Tutki_Seina(PK2Sprite &sprite, PK2BLOCK &palikka){
	int mask_index;

	//If sprite is in the block
	if (sprite_x <= palikka.oikea && sprite_x >= palikka.vasen && sprite_y <= palikka.ala && sprite_y >= palikka.yla){

		/**********************************************************************/
		/* Examine if block is water background                               */
		/**********************************************************************/
		if (palikka.vesi)
			sprite.vedessa = true;

		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_TULI && kytkin1 == 0 && sprite.isku == 0){
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
		}

		/**********************************************************************/
		/* Examine if bloc is hideway                                         */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_PIILO)
			sprite.piilossa = true;

		/**********************************************************************/
		/* Examine if block is the exit                                       */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_LOPETUS && sprite.pelaaja != 0){
			if (!jakso_lapaisty){
				if (PisteSound_StartMusic("music/hiscore.xm")!=0)
					PK2_virhe = true;
				jakso_lapaisty = true;
				jaksot[jakso_indeksi_nyt].lapaisty = true;
				if (jaksot[jakso_indeksi_nyt].jarjestys >= jakso)
					jakso++; //Increase level
				musiikin_voimakkuus = musiikin_max_voimakkuus;
				musiikin_voimakkuus_nyt = musiikin_max_voimakkuus-1;
			}
		}
	}

	//If sprite is thouching the block
	if (sprite_vasen <= palikka.oikea-4 && sprite_oikea >= palikka.vasen+4 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla+16){
		/**********************************************************************/
		/* Examine if it touches the fire                                     */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_TULI && kytkin1 == 0 && sprite.isku == 0){
			sprite.saatu_vahinko = 2;
			sprite.saatu_vahinko_tyyppi = VAHINKO_TULI;
		}
	}

	//Examine if there is a block on bottom
	if ((palikka.koodi<80 || palikka.koodi>139) && palikka.koodi != BLOCK_ESTO_ALAS && palikka.koodi < 150){
		mask_index = (int)(sprite_x+sprite_a) - palikka.vasen;

		if (mask_index < 0)
			mask_index = 0;

		if (mask_index > 31)
			mask_index = 31;

		palikka.yla += palikkamaskit[palikka.koodi].alas[mask_index];

		if (palikka.yla >= palikka.ala-2)
			palikka.alas = BLOCK_TAUSTA;

		palikka.ala -= palikkamaskit[palikka.koodi].ylos[mask_index];
	}

	//If sprite is thouching the block (again?)
	if (sprite_vasen <= palikka.oikea+2 && sprite_oikea >= palikka.vasen-2 && sprite_yla <= palikka.ala && sprite_ala >= palikka.yla){
		/**********************************************************************/
		/* Examine if it is a key and touches lock wall                       */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_LUKKO && sprite.tyyppi->avain){
			kartta->seinat[palikka.vasen/32+(palikka.yla/32)*PK2KARTTA_KARTTA_LEVEYS] = 255;
			PK_Kartta_Laske_Reunat();

			sprite.piilota = true;

			if (sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				avaimia--;
				if (avaimia < 1)
					PK_Kartta_Avaa_Lukot();
			}

			PK_Tehoste_Rajahdys(palikka.vasen+16, palikka.yla+10, 0);
			PK_Soita_Aani(avaa_lukko_aani,100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
		}

		/**********************************************************************/
		/* Make wind effects                                                  */
		/**********************************************************************/
		if (palikka.koodi == BLOCK_VIRTA_VASEMMALLE && vasemmalle)
			sprite_a -= 0.02;

		if (palikka.koodi == BLOCK_VIRTA_OIKEALLE && oikealle)
			sprite_a += 0.02;	//0.05

		/*********************************************************************/
		/* Examine if sprite is on the border to fall                        */
		/*********************************************************************/
		if (palikka.reuna && sprite.hyppy_ajastin <= 0 && sprite_y < palikka.ala && sprite_y > palikka.yla){
			/* && sprite_ala <= palikka.ala+2)*/ // onko sprite tullut reunalle
			if (sprite_vasen > palikka.vasen)
				sprite.reuna_vasemmalla = true;

			if (sprite_oikea < palikka.oikea)
				sprite.reuna_oikealla = true;
		}
	}

	//Examine walls on left and right

	if (sprite_yla < palikka.ala && sprite_ala-1 > palikka.yla)
	{
		if (sprite_oikea+sprite_a-1 > palikka.vasen && sprite_vasen+sprite_a < palikka.oikea)
		{
			// Examine whether the sprite going in the right side of the block.
			if (sprite_oikea+sprite_a < palikka.oikea)
			{
				// Onko palikka sein�?
				if (palikka.oikealle == BLOCK_SEINA)
				{
					oikealle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.vasen - sprite_leveys/2;
				}
			}
			// Examine whether the sprite going in the left side of the block.
			if (sprite_vasen+sprite_a > palikka.vasen)
			{
				if (palikka.vasemmalle == BLOCK_SEINA)
				{
					vasemmalle = false;

					if (palikka.koodi == BLOCK_HISSI_HORI)
						sprite_x = palikka.oikea + sprite_leveys/2;

				}
			}
		}
	}

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;

	//Examine walls on up and down

	if (sprite_vasen < palikka.oikea && sprite_oikea-1 > palikka.vasen) //Remove the left and right blocks
	{
		if (sprite_ala+sprite_b-1 >= palikka.yla && sprite_yla+sprite_b <= palikka.ala) //Remove the up and down blocks
		{
			if (sprite_ala+sprite_b-1 <= palikka.ala) //Just in the sprite's foot
			{
				if (palikka.alas == BLOCK_SEINA) //If it is a wall
				{
					alas = false;
					if (palikka.koodi == BLOCK_HISSI_VERT)
						sprite_y = palikka.yla - sprite_korkeus /2;

					if (sprite_ala-1 >= palikka.yla && sprite_b >= 0)
					{
						//sprite_y -= sprite_ala - palikka.yla;
						if (palikka.koodi != BLOCK_HISSI_HORI)
						{
							sprite_y = palikka.yla - sprite_korkeus /2;
						}
					}

					if (sprite.kytkinpaino >= 1)
					{
						if (palikka.koodi == BLOCK_KYTKIN1 && kytkin1 == 0)
						{
							kytkin1 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}

						if (palikka.koodi == BLOCK_KYTKIN2 && kytkin2 == 0)
						{
							kytkin2 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}

						if (palikka.koodi == BLOCK_KYTKIN3 && kytkin3 == 0)
						{
							kytkin3 = KYTKIN_ALOITUSARVO;
							kytkin_tarina = 64;
							PK_Soita_Aani(kytkin_aani, 100, (int)sprite_x, (int)sprite_y, AANET_SAMPLERATE, false);
						}

						//if (sprite_b > 3 && sprite.paino >= 2)
						//	kytkin_tarina = 5;
					}

				}
			}

			if (sprite_yla+sprite_b > palikka.yla)
			{
				if (palikka.ylos == BLOCK_SEINA)
				{
					ylos = false;

					if (sprite_yla < palikka.ala)
					{
						if (palikka.koodi == BLOCK_HISSI_VERT && sprite.kyykky)
						{
							sprite.saatu_vahinko = 2;
							sprite.saatu_vahinko_tyyppi = VAHINKO_ISKU;
						}

						if (palikka.koodi != BLOCK_HISSI_HORI)
						{
							//if (sprite.kyykky)
							//	sprite_y = palikka.ala + sprite_korkeus /2;

							sprite.kyykky = true;
						}
					}
				}
			}
		}
	}
}

//PK_Move_Sprite
int PK_Sprite_Liikuta(int i){

	PK2Sprite &sprite = spritet[i]; //address of sprite = address of spritet[i] (if change sprite, change spritet[i])

	if (i >= MAX_SPRITEJA || i < 0)
		return -1;

	if (!sprite.tyyppi)
		return -1;

	sprite_x = sprite.x;
	sprite_y = sprite.y;
	sprite_a = sprite.a;
	sprite_b = sprite.b;

	sprite_leveys  = sprite.tyyppi->leveys;
	sprite_korkeus = sprite.tyyppi->korkeus;

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;

	max_nopeus = (BYTE)sprite.tyyppi->max_nopeus;

	vedessa = sprite.vedessa;

	x = 0;
	y = 0;

	oikealle	 = true,
	vasemmalle	 = true,
	ylos		 = true,
	alas		 = true;

	kartta_vasen = 0;
	kartta_yla   = 0;

	sprite.kyykky = false;

	sprite.reuna_vasemmalla = false;
	sprite.reuna_oikealla = false;


	/* Pistet��n vauhtia tainnutettuihin spriteihin */
	if (sprite.energia < 1/* && sprite.tyyppi->max_nopeus == 0*/)
		max_nopeus = 3;

	// Calculate the remainder of the sprite towards

	if (sprite.hyokkays1 > 0)
		sprite.hyokkays1--;

	if (sprite.hyokkays2 > 0)
		sprite.hyokkays2--;

	if (sprite.lataus > 0)	// aika kahden ampumisen (munimisen) v�lill�
		sprite.lataus --;

	if (sprite.muutos_ajastin > 0)	// aika muutokseen
		sprite.muutos_ajastin --;

	/*****************************************************************************************/
	/* Player-sprite and its controls                                                        */
	/*****************************************************************************************/

	bool lisavauhti = true;
	bool hidastus = false;

	PisteInput_Lue_Eventti();
	if (sprite.pelaaja != 0 && sprite.energia > 0)
	{
		/* WALK */
		if (PisteInput_Keydown(kontrolli_juoksu))
			lisavauhti = false;

		/* ATTACK 1 */
		if (PisteInput_Keydown(kontrolli_hyokkays1) && sprite.lataus == 0 && sprite.ammus1 != -1) {
			sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;
		}
		/* ATTACK 2 */
		else {
			if (PisteInput_Keydown(kontrolli_hyokkays2) && sprite.lataus == 0 && sprite.ammus2 != -1)
				sprite.hyokkays2 = sprite.tyyppi->hyokkays2_aika;
		}

		/* CROUCH */
		sprite.kyykky = false;

		if (PisteInput_Keydown(kontrolli_alas) && !sprite.alas)
		{
			sprite.kyykky = true;
			sprite_yla += sprite_korkeus/1.5;
		}

		double a_lisays = 0;

		/* NAVIGATING TO RIGHT */
		if (PisteInput_Keydown(kontrolli_oikealle))
		{
			a_lisays = 0.04;//0.08;

			if (lisavauhti)
			{
				if (rand()%20 == 1 && sprite.animaatio_index == ANIMAATIO_KAVELY) //rand()%30
					PK_Partikkeli_Uusi(PARTIKKELI_POLYPILVI,sprite_x-8,sprite_ala-8,0.25,-0.25,40,0,0);

				a_lisays += 0.09;//0.05
			}

			if (sprite.alas)
				a_lisays /= 1.5;//2.0

			sprite.flip_x = false;
		}

		/* NAVIGATING TO LEFT */
		if (PisteInput_Keydown(kontrolli_vasemmalle))
		{
			a_lisays = -0.04;

			if (lisavauhti)
			{
				if (rand()%20 == 1 && sprite.animaatio_index == ANIMAATIO_KAVELY)
				{
					PK_Partikkeli_Uusi(PARTIKKELI_POLYPILVI,sprite_x-8,sprite_ala-8,-0.25,-0.25,40,0,0);
				}

				a_lisays -= 0.09;
			}

			if (sprite.alas)	// spriten koskettaessa maata kitka vaikuttaa
				a_lisays /= 1.5;//2.0

			sprite.flip_x = true;
		}

		if (sprite.kyykky)	// Slow when couch

			a_lisays /= 10;

		sprite_a += a_lisays;

		/* JUMPING */
		if (sprite.tyyppi->paino > 0)
		{
			if (PisteInput_Keydown(kontrolli_hyppy))
			{
				if (!sprite.kyykky)
				{
					if (sprite.hyppy_ajastin == 0)
						PK_Soita_Aani(hyppy_aani, 100, (int)sprite_x, (int)sprite_y,
									  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

					if (sprite.hyppy_ajastin <= 0)
						sprite.hyppy_ajastin = 1; //10;
				}
			}
			else
			{
				/*
				if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < 55)
					sprite.hyppy_ajastin = 55; */

				if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < 45)//40
					sprite.hyppy_ajastin = 55;
			}

			/* tippuminen hiljaa alasp�in */
			if (PisteInput_Keydown(kontrolli_hyppy) && sprite.hyppy_ajastin >= 150/*90+20*/ &&
				sprite.tyyppi->liitokyky)
				hidastus = true;
		}
		/* MOVING UP AND DOWN */
		else{ // if the player sprite-weight is 0 - like birds

			if (PisteInput_Keydown(kontrolli_hyppy))
				sprite_b -= 0.15;

			if (PisteInput_Keydown(kontrolli_alas))
				sprite_b += 0.15;

			sprite.hyppy_ajastin = 0;
		}

		/* AI */
		for (int ai=0;ai < SPRITE_MAX_AI;ai++)
			switch (sprite.tyyppi->AI[ai]){
			case AI_MUUTOS_JOS_ENERGIAA_ALLE_2:	if (sprite.tyyppi->muutos > -1)
													sprite.AI_Muutos_Jos_Energiaa_Alle_2(protot[sprite.tyyppi->muutos]);
												break;
			case AI_MUUTOS_JOS_ENERGIAA_YLI_1:	if (sprite.tyyppi->muutos > -1)
												{
													if (sprite.AI_Muutos_Jos_Energiaa_Yli_1(protot[sprite.tyyppi->muutos])==1)
														PK_Tehosteet(TUHOUTUMINEN_SAVU_HARMAA, (DWORD)sprite.x, (DWORD)sprite.y);
												}
												break;
			case AI_MUUTOS_AJASTIN:				if (sprite.tyyppi->muutos > -1)
													sprite.AI_Muutos_Ajastin(protot[sprite.tyyppi->muutos]);
												break;
			case AI_VAHINGOITTUU_VEDESTA:		sprite.AI_Vahingoittuu_Vedesta();break;

			case AI_MUUTOS_JOS_OSUTTU:			if (sprite.tyyppi->muutos > -1) {
													sprite.AI_Muutos_Jos_Osuttu(protot[sprite.tyyppi->muutos]);
												}
												break;
			default:			break;
			}

		/* It is not acceptable that a player is anything other than the game character */
		if (sprite.tyyppi->tyyppi != TYYPPI_PELIHAHMO)
			sprite.energia = 0;
	}

	/*****************************************************************************************/
	/* Jump                                                                                  */
	/*****************************************************************************************/

	bool hyppy_maximissa = sprite.hyppy_ajastin >= 90;

	// Jos ollaan hyp�tty / ilmassa:
	if (sprite.hyppy_ajastin > 0)
	{
		if (sprite.hyppy_ajastin < 50-sprite.tyyppi->max_hyppy)
			sprite.hyppy_ajastin = 50-sprite.tyyppi->max_hyppy;

		if (sprite.hyppy_ajastin < 10)
			sprite.hyppy_ajastin = 10;

		if (!hyppy_maximissa){
		// sprite_b = (sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.0;//-4
		   sprite_b = -sin_table[sprite.hyppy_ajastin]/8;//(sprite.tyyppi->max_hyppy/2 - sprite.hyppy_ajastin/2)/-2.5;
			if (sprite_b > sprite.tyyppi->max_hyppy){
				sprite_b = sprite.tyyppi->max_hyppy/10.0;
				sprite.hyppy_ajastin = 90 - sprite.hyppy_ajastin;
			}

		}

		if (sprite.hyppy_ajastin < 180)
			sprite.hyppy_ajastin += 2;
	}

	if (sprite.hyppy_ajastin < 0)
		sprite.hyppy_ajastin++;

	if (sprite_b > 0 && !hyppy_maximissa)
		sprite.hyppy_ajastin = 90;//sprite.tyyppi->max_hyppy*2;

	/*****************************************************************************************/
	/* Hit recovering                                                                        */
	/*****************************************************************************************/

	if (sprite.isku > 0)
		sprite.isku --;

	/*****************************************************************************************/
	/* Gravity effect                                                                        */
	/*****************************************************************************************/

	if (sprite.paino != 0 && (sprite.hyppy_ajastin <= 0 || sprite.hyppy_ajastin >= 45))
		sprite_b += sprite.paino/1.25;// + sprite_b/1.5;

	if (hidastus && sprite_b > 0) // If gliding
		sprite_b /= 1.3;//1.5;//3

	/*****************************************************************************************/
	/* By default, the sprite is not in the water and not hidden                             */
	/*****************************************************************************************/

	sprite.vedessa  = false;
	sprite.piilossa = false;

	/*****************************************************************************************/
	/* Speed limits                                                                          */
	/*****************************************************************************************/

	if (sprite_b > 4.0)//4
		sprite_b = 4.0;//4

	if (sprite_b < -4.0)
		sprite_b = -4.0;

	if (sprite_a > max_nopeus)
		sprite_a = max_nopeus;

	if (sprite_a < -max_nopeus)
		sprite_a = -max_nopeus;

	/*****************************************************************************************/
	/* Blocks colision -                                                                     */
	/*****************************************************************************************/

	int palikat_x_lkm,
	    palikat_y_lkm,
	    palikat_lkm;
	DWORD p;

	if (sprite.tyyppi->tiletarkistus){ //Find the tiles that the sprite occupies

		palikat_x_lkm = (int)((sprite_leveys) /32)+4; //Number of blocks
		palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

		kartta_vasen = (int)(sprite_vasen)/32;	//Position in tile map
		kartta_yla	 = (int)(sprite_yla)/32;

		for (y=0;y<palikat_y_lkm;y++)
			for (x=0;x<palikat_x_lkm;x++) //For each block, create a array of blocks around the sprite
				palikat[x+(y*palikat_x_lkm)] = PK_Palikka_Tutki(kartta_vasen+x-1,kartta_yla+y-1); //x = 0, y = 0

		/*****************************************************************************************/
		/* Going through the blocks around the sprite.                                           */
		/*****************************************************************************************/

		palikat_lkm = palikat_y_lkm*palikat_x_lkm;
		for (y=0;y<palikat_y_lkm;y++){
			for (x=0;x<palikat_x_lkm;x++) {
				p = x+y*palikat_x_lkm;
				if (p<300 && p>=0)//{
					//if(sprite.pelaaja == 1) printf("%i\n",palikat_lkm);
					PK_Tutki_Seina(sprite, palikat[p]);
				//}
			}
		}
	}
	/*****************************************************************************************/
	/* If the sprite is under water                                                          */
	/*****************************************************************************************/

	if (sprite.vedessa)
	{

		if (!sprite.tyyppi->osaa_uida || sprite.energia < 1)
		{
			/*
			if (sprite_b > 0)
				sprite_b /= 2.0;

			sprite_b -= (1.5-sprite.paino)/10;*/
			sprite_b /= 2.0;
			sprite_a /= 1.05;

			if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < 90)
				sprite.hyppy_ajastin--;
		}

		if (rand()%80 == 1)
			PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
	}

	if (vedessa != sprite.vedessa) // Sprite comes in or out from water
	{
		PK_Tehoste_Loiskahdus((int)sprite_x,(int)sprite_y,32);
	}

	/*****************************************************************************************/
	/* Sprite weight                                                                         */
	/*****************************************************************************************/

	sprite.paino = sprite.alkupaino;
	sprite.kytkinpaino = sprite.paino;

	if (sprite.energia < 1 && sprite.paino == 0)
		sprite.paino = 1;

	/*****************************************************************************************/
	/* Sprite collision with other sprites                                                   */
	/*****************************************************************************************/

	int tuhoutuminen;
	double sprite2_yla; // kyykistymiseen liittyv�
	PK2BLOCK spritepalikka;

	PK2Sprite *sprite2;

	for (int sprite_index = 0; sprite_index < MAX_SPRITEJA; sprite_index++)
	{
		sprite2 = &spritet[sprite_index];

		if (sprite_index != i && /*!sprite2->piilota*/sprite2->aktiivinen)
		{
			if (sprite2->kyykky)
				sprite2_yla = sprite2->tyyppi->korkeus / 3;//1.5;
			else
				sprite2_yla = 0;

			if (sprite2->tyyppi->este && sprite.tyyppi->tiletarkistus) //If there is a block sprite active
			{

				if (sprite_x-sprite_leveys/2 +sprite_a  <= sprite2->x + sprite2->tyyppi->leveys /2 &&
					sprite_x+sprite_leveys/2 +sprite_a  >= sprite2->x - sprite2->tyyppi->leveys /2 &&
					sprite_y-sprite_korkeus/2+sprite_b <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
					sprite_y+sprite_korkeus/2+sprite_b >= sprite2->y - sprite2->tyyppi->korkeus/2)
				{
					spritepalikka.koodi = 0;
					spritepalikka.ala   = (int)sprite2->y + sprite2->tyyppi->korkeus/2;
					spritepalikka.oikea = (int)sprite2->x + sprite2->tyyppi->leveys/2;
					spritepalikka.vasen = (int)sprite2->x - sprite2->tyyppi->leveys/2;
					spritepalikka.yla   = (int)sprite2->y - sprite2->tyyppi->korkeus/2;

					spritepalikka.vesi  = false;

					//spritepalikka.koodi = BLOCK_HISSI_VERT;
					/*
					PK_Palikka_Este(spritepalikka);

					if (!sprite.tyyppi->este)
					{
						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas		 = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos		 = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle   = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;
					}
					*/

					PK_Palikka_Este(spritepalikka);

					if (!sprite.tyyppi->este){
						if (!sprite2->tyyppi->este_alas)
							spritepalikka.alas = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_ylos)
							spritepalikka.ylos = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_oikealle)
							spritepalikka.oikealle = BLOCK_TAUSTA;
						if (!sprite2->tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;
					}

					if (sprite2->a > 0)
						spritepalikka.koodi = BLOCK_HISSI_HORI;

					if (sprite2->b > 0)
						spritepalikka.koodi = BLOCK_HISSI_VERT;

					PK_Tutki_Seina2(sprite, spritepalikka); //Colision sprite and sprite block
				}
			}

			if (sprite_x <= sprite2->x + sprite2->tyyppi->leveys /2 &&
				sprite_x >= sprite2->x - sprite2->tyyppi->leveys /2 &&
				sprite_y/*yla*/ <= sprite2->y + sprite2->tyyppi->korkeus/2 &&
				sprite_y/*ala*/ >= sprite2->y - sprite2->tyyppi->korkeus/2 + sprite2_yla)
			{
				// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
				if (sprite.tyyppi->indeksi == sprite2->tyyppi->indeksi &&
					sprite2->energia > 0/* && sprite.pelaaja == 0*/)
				{
					if (sprite.x < sprite2->x)
						oikealle = false;
					if (sprite.x > sprite2->x)
						vasemmalle = false;
					if (sprite.y < sprite2->y)
						alas = false;
					if (sprite.y > sprite2->y)
						ylos = false;
				}

				if (sprite.tyyppi->Onko_AI(AI_NUOLET_VAIKUTTAVAT)) {

					if (sprite2->tyyppi->Onko_AI(AI_NUOLI_OIKEALLE)) {
						sprite_a = sprite.tyyppi->max_nopeus / 3.5;
						sprite_b = 0;
					}
					else if (sprite2->tyyppi->Onko_AI(AI_NUOLI_VASEMMALLE)) {
						sprite_a = sprite.tyyppi->max_nopeus / -3.5;
						sprite_b = 0;
					}

					if (sprite2->tyyppi->Onko_AI(AI_NUOLI_YLOS)) {
						sprite_b = sprite.tyyppi->max_nopeus / -3.5;
						sprite_a = 0;
					}
					else if (sprite2->tyyppi->Onko_AI(AI_NUOLI_ALAS)) {
						sprite_b = sprite.tyyppi->max_nopeus / 3.5;
						sprite_a = 0;
					}
				}

				/* spritet voivat vaihtaa tietoa pelaajan olinpaikasta */
	/*			if (sprite.pelaaja_x != -1 && sprite2->pelaaja_x == -1)
				{
					sprite2->pelaaja_x = sprite.pelaaja_x + rand()%30 - rand()%30;
					sprite.pelaaja_x = -1;
				} */


				if (sprite.vihollinen != sprite2->vihollinen && sprite.emosprite != sprite_index)
				{
					if (sprite2->tyyppi->tyyppi != TYYPPI_TAUSTA &&
						sprite.tyyppi->tyyppi   != TYYPPI_TAUSTA &&
						sprite2->tyyppi->tyyppi != TYYPPI_TELEPORTTI &&
						sprite2->isku == 0 &&
						sprite.isku == 0 &&
						sprite2->energia > 0 &&
						sprite.energia > 0 &&
						sprite2->saatu_vahinko < 1)
					{

						// Tippuuko toinen sprite p��lle?

						if (sprite2->b > 2 && sprite2->paino >= 0.5 &&
							sprite2->y < sprite_y && !sprite.tyyppi->este &&
							sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
						{
							//sprite.saatu_vahinko = (int)sprite2->paino;//1;
							sprite.saatu_vahinko = (int)(sprite2->paino+sprite2->b/4);
							sprite.saatu_vahinko_tyyppi = VAHINKO_PUDOTUS;
							sprite2->hyppy_ajastin = 1;
						}

						// Is there another sprite damaging

						if (sprite.tyyppi->vahinko > 0 && sprite2->tyyppi->tyyppi != TYYPPI_BONUS)
						{
							sprite2->saatu_vahinko		  = sprite.tyyppi->vahinko;
							sprite2->saatu_vahinko_tyyppi = sprite.tyyppi->vahinko_tyyppi;
							sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;

							// Ammukset hajoavat t�rm�yksest�

							if (sprite2->tyyppi->tyyppi == TYYPPI_AMMUS)
							{
								sprite.saatu_vahinko = 1;//sprite2->tyyppi->vahinko;
								sprite.saatu_vahinko_tyyppi = sprite2->tyyppi->vahinko_tyyppi;
							}

							if (sprite.tyyppi->tyyppi == TYYPPI_AMMUS)
							{
								sprite.saatu_vahinko = 1;//sprite2->tyyppi->vahinko;
								sprite.saatu_vahinko_tyyppi = sprite2->tyyppi->vahinko_tyyppi;
							}
						}
					}
				}

				// lis�t��n spriten painoon sit� koskettavan toisen spriten paino
				if (sprite.paino > 0)
					sprite.kytkinpaino += spritet[sprite_index].tyyppi->paino;

			}
		}
	}

	/*****************************************************************************************/
	/* If the sprite has suffered damage                                                     */
	/*****************************************************************************************/

	// jos n�kym�t�n, ei saa damagea kuin tulesta.
	if (nakymattomyys > 0 && sprite.saatu_vahinko != 0 && sprite.saatu_vahinko_tyyppi != VAHINKO_TULI &&
		i == pelaaja_index) {
		sprite.saatu_vahinko = 0;
		sprite.saatu_vahinko_tyyppi = VAHINKO_EI;
	}



	if (sprite.saatu_vahinko != 0 && sprite.energia > 0 && sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
	{
		if (sprite.tyyppi->suojaus != sprite.saatu_vahinko_tyyppi || sprite.tyyppi->suojaus == VAHINKO_EI)
		{
			sprite.energia -= sprite.saatu_vahinko;
			sprite.isku = VAHINKO_AIKA;

			if (sprite.saatu_vahinko_tyyppi == VAHINKO_SAHKO)
				sprite.isku *= 6;

			PK_Soita_Aani(sprite.tyyppi->aanet[AANI_VAHINKO], 100, (int)sprite.x, (int)sprite.y,
						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			if (sprite.tyyppi->tuhoutuminen%100 == TUHOUTUMINEN_HOYHENET)
			{
				PK_Tehosteet(TUHOUTUMINEN_HOYHENET, (DWORD)sprite.x, (DWORD)sprite.y);
			}

			if (sprite.tyyppi->tyyppi != TYYPPI_AMMUS)
			{
				PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,sprite_x,sprite_y,-1,-1,60,0.01,128);
				PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,sprite_x,sprite_y, 0,-1,60,0.01,128);
				PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,sprite_x,sprite_y, 1,-1,60,0.01,128);
			}

			if (sprite.Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
				PK_Kartta_Vaihda_Kallopalikat();

			if (sprite.Onko_AI(AI_HYOKKAYS_1_JOS_OSUTTU))
			{
				sprite.hyokkays1 = sprite.tyyppi->hyokkays1_aika;
				sprite.lataus = 0;
			}

			if (sprite.Onko_AI(AI_HYOKKAYS_2_JOS_OSUTTU))
			{
				sprite.hyokkays2 = sprite.tyyppi->hyokkays2_aika;
				sprite.lataus = 0;
			}

		}

		sprite.saatu_vahinko = 0;
		sprite.saatu_vahinko_tyyppi = VAHINKO_EI;


		/*****************************************************************************************/
		/* If the sprite is destroyed                                                            */
		/*****************************************************************************************/

		if (sprite.energia < 1)
		{
			tuhoutuminen = sprite.tyyppi->tuhoutuminen;

			if (tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				if (sprite.tyyppi->bonus > -1 && sprite.tyyppi->bonusten_lkm > 0)
					if (sprite.tyyppi->bonus_aina || rand()%4 == 1)
						for (int bi=0; bi<sprite.tyyppi->bonusten_lkm; bi++)
							PK_Sprite_Lisaa_Bonus(protot[sprite.tyyppi->bonus],0,sprite_x-11+(10-rand()%20),
											  sprite_ala-16-(10+rand()%20), i);

				if (sprite.Onko_AI(AI_VAIHDA_KALLOT_JOS_TYRMATTY) && !sprite.Onko_AI(AI_VAIHDA_KALLOT_JOS_OSUTTU))
					PK_Kartta_Vaihda_Kallopalikat();

				if (tuhoutuminen >= TUHOUTUMINEN_ANIMAATIO)
					tuhoutuminen -= TUHOUTUMINEN_ANIMAATIO;
				else
					sprite.piilota = true;

				PK_Tehosteet(tuhoutuminen, (DWORD)sprite.x, (DWORD)sprite.y);
				PK_Soita_Aani(sprite.tyyppi->aanet[AANI_TUHOUTUMINEN],100, (int)sprite.x, (int)sprite.y,
							  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

				if (sprite.Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					PK_Sprite_Lisaa(*sprite.tyyppi,0,sprite.alku_x-sprite.tyyppi->leveys, sprite.alku_y-sprite.tyyppi->korkeus,i);
				}

				if (sprite.tyyppi->tyyppi == TYYPPI_PELIHAHMO && sprite.tyyppi->pisteet != 0)
				{
					char luku[10];
					itoa(sprite.tyyppi->pisteet,luku,10);
					PK_Fadeteksti_Uusi(fontti2,luku,(int)spritet[i].x-8,(int)spritet[i].y-8,80);
					piste_lisays += sprite.tyyppi->pisteet;
				}
			}
			else
				sprite.energia = 1;
		}
	}

	if (sprite.isku == 0)
		sprite.saatu_vahinko_tyyppi = VAHINKO_EI;


	/*****************************************************************************************/
	/* Revisions                                                                             */
	/*****************************************************************************************/

	if (!oikealle)
	{
		if (sprite_a > 0)
			sprite_a = 0;
	}

	if (!vasemmalle)
	{
		if (sprite_a < 0)
			sprite_a = 0;
	}

	if (!ylos)
	{
		if (sprite_b < 0)
			sprite_b = 0;

		if (!hyppy_maximissa)
			sprite.hyppy_ajastin = 95;//sprite.tyyppi->max_hyppy * 2;
	}

	if (!alas)
	{
		if (sprite_b >= 0) //If sprite is falling
		{
			if (sprite.hyppy_ajastin > 0)
			{
				if (sprite.hyppy_ajastin >= 90+10)
				{
					PK_Soita_Aani(tomahdys_aani,30,(int)sprite_x, (int)sprite_y,
				                  int(25050-sprite.paino*3000),true);

					//PK_Partikkeli_Uusi(	PARTIKKELI_POLYPILVI,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
					//			  0,-0.2,rand()%50+20,0,0);

					if (rand()%7 == 1) {
						PK_Partikkeli_Uusi(PARTIKKELI_SAVUPILVI,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   0.3,-0.1,450,0,0);
						PK_Partikkeli_Uusi(PARTIKKELI_SAVUPILVI,sprite_x+rand()%5-rand()%5-10,sprite_ala+rand()%3-rand()%3,
									  	   -0.3,-0.1,450,0,0);
					}

					if (sprite.paino > 1)
						jaristys = 34 + int(sprite.paino * 20);
				}

				sprite.hyppy_ajastin = 0;
			}

			sprite_b = 0;
		}

	}

	/*****************************************************************************************/
	/* Set correct values                                                                    */
	/*****************************************************************************************/

	if (sprite_b > 4.0)
		sprite_b = 4.0;

	if (sprite_b < -4.0)
		sprite_b = -4.0;

	if (sprite_a > max_nopeus)
		sprite_a = max_nopeus;

	if (sprite_a < -max_nopeus)
		sprite_a = -max_nopeus;

	if (sprite.energia > sprite.tyyppi->energia)
		sprite.energia = sprite.tyyppi->energia;

	if (sprite.isku == 0 || sprite.pelaaja == 1)
	{
		sprite_x += sprite_a;
		sprite_y += sprite_b;
	}

	if (i == pelaaja_index || sprite.energia < 1)
	{
		double kitka = 1.04;

		if (kartta->ilma == ILMA_SADE || kartta->ilma == ILMA_SADEMETSA)
			kitka = 1.03;

		if (kartta->ilma == ILMA_LUMISADE)
			kitka = 1.01;

		if (!alas)
			sprite_a /= kitka;
		else
			sprite_a /= 1.03;//1.02//1.05

		sprite_b /= 1.25;
	}

	sprite.x = sprite_x;
	sprite.y = sprite_y;
	sprite.a = sprite_a;
	sprite.b = sprite_b;

	sprite.oikealle = oikealle;
	sprite.vasemmalle = vasemmalle;
	sprite.alas = alas;
	sprite.ylos = ylos;

	/*
	sprite.paino = sprite.tyyppi->paino;

	if (sprite.energia < 1 && sprite.paino == 0)
		sprite.paino = 1;*/

	if (sprite.hyppy_ajastin < 0)
		sprite.alas = false;

	//sprite.kyykky   = false;

	/*****************************************************************************************/
	/* AI                                                                                    */
	/*****************************************************************************************/

	if (sprite.pelaaja == 0)
	{
		for (int ai=0;ai < SPRITE_MAX_AI; ai++)
			switch (sprite.tyyppi->AI[ai])
			{
			case AI_EI:							ai = SPRITE_MAX_AI; // lopetetaan
												break;
			case AI_KANA:						sprite.AI_Kana();
												break;
			case AI_PIKKUKANA:					sprite.AI_Kana();
												break;
			case AI_SAMMAKKO1:					sprite.AI_Sammakko1();
												break;
			case AI_SAMMAKKO2:					sprite.AI_Sammakko2();
												break;
			case AI_BONUS:						sprite.AI_Bonus();
												break;
			case AI_MUNA:						sprite.AI_Muna();
												break;
			case AI_AMMUS:						sprite.AI_Ammus();
												break;
			case AI_HYPPIJA:					sprite.AI_Hyppija();
												break;
			case AI_PERUS:						sprite.AI_Perus();
												break;
			case AI_NONSTOP:					sprite.AI_NonStop();
												break;
			case AI_KAANTYY_ESTEESTA_HORI:		sprite.AI_Kaantyy_Esteesta_Hori();
												break;
			case AI_KAANTYY_ESTEESTA_VERT:		sprite.AI_Kaantyy_Esteesta_Vert();
												break;
			case AI_VAROO_KUOPPAA:				sprite.AI_Varoo_Kuoppaa();
												break;
			case AI_RANDOM_SUUNNANVAIHTO_HORI:	sprite.AI_Random_Suunnanvaihto_Hori();
												break;
			case AI_RANDOM_KAANTYMINEN:			sprite.AI_Random_Kaantyminen();
												break;
			case AI_RANDOM_HYPPY:				sprite.AI_Random_Hyppy();
												break;
			case AI_SEURAA_PELAAJAA:			if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_JOS_NAKEE:	if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa_Jos_Nakee(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_VERT_HORI:	if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa_Vert_Hori(spritet[pelaaja_index]);
												break;
			case AI_SEURAA_PELAAJAA_JOS_NAKEE_VERT_HORI:
												if (nakymattomyys == 0)
													sprite.AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(spritet[pelaaja_index]);
												break;
			case AI_PAKENEE_PELAAJAA_JOS_NAKEE:	if (nakymattomyys == 0)
													sprite.AI_Pakenee_Pelaajaa_Jos_Nakee(spritet[pelaaja_index]);
												break;
			case AI_POMMI:						sprite.AI_Pommi();
												break;
			case AI_HYOKKAYS_1_JOS_OSUTTU:		sprite.AI_Hyokkays_1_Jos_Osuttu();
												break;
			case AI_HYOKKAYS_2_JOS_OSUTTU:		sprite.AI_Hyokkays_2_Jos_Osuttu();
												break;
			case AI_HYOKKAYS_1_NONSTOP:			sprite.AI_Hyokkays_1_Nonstop();
												break;
			case AI_HYOKKAYS_2_NONSTOP:			sprite.AI_Hyokkays_2_Nonstop();
												break;
			case AI_HYOKKAYS_1_JOS_PELAAJA_EDESSA:
												if (nakymattomyys == 0)
													sprite.AI_Hyokkays_1_Jos_Pelaaja_Edessa(spritet[pelaaja_index]);
												break;
			case AI_HYOKKAYS_2_JOS_PELAAJA_EDESSA:
												if (nakymattomyys == 0)
													sprite.AI_Hyokkays_2_Jos_Pelaaja_Edessa(spritet[pelaaja_index]);
												break;
			case AI_HYOKKAYS_1_JOS_PELAAJA_ALAPUOLELLA:
												if (nakymattomyys == 0)
													sprite.AI_Hyokkays_1_Jos_Pelaaja_Alapuolella(spritet[pelaaja_index]);
												break;
			case AI_HYPPY_JOS_PELAAJA_YLAPUOLELLA:
												if (nakymattomyys == 0)
													sprite.AI_Hyppy_Jos_Pelaaja_Ylapuolella(spritet[pelaaja_index]);
												break;
			case AI_VAHINGOITTUU_VEDESTA:		sprite.AI_Vahingoittuu_Vedesta();
												break;
			case AI_TAPA_KAIKKI:				sprite.AI_Tapa_Kaikki();
												break;
			case AI_KITKA_VAIKUTTAA:			sprite.AI_Kitka_Vaikuttaa();
												break;
			case AI_PIILOUTUU:					sprite.AI_Piiloutuu();
												break;
			case AI_PALAA_ALKUUN_X:				sprite.AI_Palaa_Alkuun_X();
												break;
			case AI_PALAA_ALKUUN_Y:				sprite.AI_Palaa_Alkuun_Y();
												break;
			case AI_LIIKKUU_X_COS:				sprite.AI_Liikkuu_X(cos_table[degree%360]);
												break;
			case AI_LIIKKUU_Y_COS:				sprite.AI_Liikkuu_Y(cos_table[degree%360]);
												break;
			case AI_LIIKKUU_X_SIN:				sprite.AI_Liikkuu_X(sin_table[degree%360]);
												break;
			case AI_LIIKKUU_Y_SIN:				sprite.AI_Liikkuu_Y(sin_table[degree%360]);
												break;
			case AI_LIIKKUU_X_COS_NOPEA:		sprite.AI_Liikkuu_X(cos_table[(degree*2)%360]);
												break;
			case AI_LIIKKUU_Y_SIN_NOPEA:		sprite.AI_Liikkuu_Y(sin_table[(degree*2)%360]);
												break;
			case AI_LIIKKUU_X_COS_HIDAS:		sprite.AI_Liikkuu_X(cos_table[(degree/2)%360]);
												break;
			case AI_LIIKKUU_Y_SIN_HIDAS:		sprite.AI_Liikkuu_Y(sin_table[(degree/2)%360]);
												break;
			case AI_LIIKKUU_Y_SIN_VAPAA:		sprite.AI_Liikkuu_Y(sin_table[(sprite.ajastin/2)%360]);
												break;
			case AI_MUUTOS_JOS_ENERGIAA_ALLE_2:	if (sprite.tyyppi->muutos > -1)
													sprite.AI_Muutos_Jos_Energiaa_Alle_2(protot[sprite.tyyppi->muutos]);
												break;
			case AI_MUUTOS_JOS_ENERGIAA_YLI_1:	if (sprite.tyyppi->muutos > -1)
													if (sprite.AI_Muutos_Jos_Energiaa_Yli_1(protot[sprite.tyyppi->muutos])==1)
														PK_Tehosteet(TUHOUTUMINEN_SAVU_HARMAA, (DWORD)sprite.x, (DWORD)sprite.y);
												break;
			case AI_MUUTOS_AJASTIN:				if (sprite.tyyppi->muutos > -1) {
													sprite.AI_Muutos_Ajastin(protot[sprite.tyyppi->muutos]);
												}
												break;
			case AI_MUUTOS_JOS_OSUTTU:			if (sprite.tyyppi->muutos > -1) {
													sprite.AI_Muutos_Jos_Osuttu(protot[sprite.tyyppi->muutos]);
												}
												break;
			case AI_TELEPORTTI:					if (sprite.AI_Teleportti(i, spritet, MAX_SPRITEJA, spritet[pelaaja_index])==1)
												{

													kamera_x = (int)spritet[pelaaja_index].x;
													kamera_y = (int)spritet[pelaaja_index].y;
													dkamera_x = kamera_x-KARTANPIIRTO_LEVEYS/2;
													dkamera_y = kamera_y-KARTANPIIRTO_KORKEUS/2;
													PisteDraw2_FadeIn(PD_FADE_NORMAL);
													if (sprite.tyyppi->aanet[AANI_HYOKKAYS2] != -1)
														PK_Soita_Aani_Menu(sprite.tyyppi->aanet[AANI_HYOKKAYS2], 100);
														//PK_Soita_Aani(, 100, kamera_x, kamera_y, AANET_SAMPLERATE, false);


												}
												break;
			case AI_KIIPEILIJA:					sprite.AI_Kiipeilija();
												break;
			case AI_KIIPEILIJA2:				sprite.AI_Kiipeilija2();
												break;
			case AI_TUHOUTUU_JOS_EMO_TUHOUTUU:	sprite.AI_Tuhoutuu_Jos_Emo_Tuhoutuu(spritet);
												break;

			case AI_TIPPUU_TARINASTA:			sprite.AI_Tippuu_Tarinasta(jaristys + kytkin_tarina);
												break;
			case AI_LIIKKUU_ALAS_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,0,1);
												break;
			case AI_LIIKKUU_YLOS_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,0,-1);
												break;
			case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,-1,0);
												break;
			case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN1_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin1,1,0);
												break;
			case AI_LIIKKUU_ALAS_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,0,1);
												break;
			case AI_LIIKKUU_YLOS_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,0,-1);
												break;
			case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,-1,0);
												break;
			case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN2_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin2,1,0);
												break;
			case AI_LIIKKUU_ALAS_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,0,1);
												break;
			case AI_LIIKKUU_YLOS_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,0,-1);
												break;
			case AI_LIIKKUU_VASEMMALLE_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,-1,0);
												break;
			case AI_LIIKKUU_OIKEALLE_JOS_KYTKIN3_PAINETTU: sprite.AI_Liikkuu_Jos_Kytkin_Painettu(kytkin3,1,0);
												break;
			case AI_TIPPUU_JOS_KYTKIN1_PAINETTU: sprite.AI_Tippuu_Jos_Kytkin_Painettu(kytkin1);
												break;
			case AI_TIPPUU_JOS_KYTKIN2_PAINETTU: sprite.AI_Tippuu_Jos_Kytkin_Painettu(kytkin2);
												break;
			case AI_TIPPUU_JOS_KYTKIN3_PAINETTU: sprite.AI_Tippuu_Jos_Kytkin_Painettu(kytkin3);
												break;
			case AI_RANDOM_LIIKAHDUS_VERT_HORI:	sprite.AI_Random_Liikahdus_Vert_Hori();
												break;
			case AI_KAANTYY_JOS_OSUTTU:			sprite.AI_Kaantyy_Jos_Osuttu();
												break;
			case AI_EVIL_ONE:					if (sprite.energia < 1) musiikin_voimakkuus = 0;
												break;

			case AI_INFO1:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info01));break;
			case AI_INFO2:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info02));break;
			case AI_INFO3:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info03));break;
			case AI_INFO4:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info04));break;
			case AI_INFO5:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info05));break;
			case AI_INFO6:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info06));break;
			case AI_INFO7:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info07));break;
			case AI_INFO8:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info08));break;
			case AI_INFO9:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info09));break;
			case AI_INFO10:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info10));break;
			case AI_INFO11:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info11));break;
			case AI_INFO12:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info12));break;
			case AI_INFO13:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info13));break;
			case AI_INFO14:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info14));break;
			case AI_INFO15:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info15));break;
			case AI_INFO16:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info16));break;
			case AI_INFO17:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info17));break;
			case AI_INFO18:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info18));break;
			case AI_INFO19:						if (sprite.AI_Info(spritet[pelaaja_index]))	PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_info19));break;

			default:							break;
			}
	}

	//if (kaiku == 1 && sprite.tyyppi->tyyppi == TYYPPI_AMMUS && sprite.tyyppi->vahinko_tyyppi == VAHINKO_MELU &&
	//	sprite.tyyppi->aanet[AANI_HYOKKAYS1] > -1)
	//	PK_Soita_Aani(sprite.tyyppi->aanet[AANI_HYOKKAYS1],20, (int)sprite_x, (int)sprite_y,
	//				  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);


	/*****************************************************************************************/
	/* Set game area to sprite                                                               */
	/*****************************************************************************************/

	if (sprite.x < 0)
		sprite.x = 0;

	if (sprite.y < -sprite_korkeus)
		sprite.y = -sprite_korkeus;

	if (sprite.x > PK2KARTTA_KARTTA_LEVEYS*32)
		sprite.x = PK2KARTTA_KARTTA_LEVEYS*32;

	// If the sprite falls under the lower edge of the map
	if (sprite.y > PK2KARTTA_KARTTA_KORKEUS*32+sprite_korkeus)
	{
		sprite.y = PK2KARTTA_KARTTA_KORKEUS*32+sprite_korkeus;
		sprite.energia = 0;
		sprite.piilota = true;

		if (sprite.kytkinpaino >= 1)
			jaristys = 50;
	}

	if (sprite.a > max_nopeus)
		sprite.a = max_nopeus;

	if (sprite.a < -max_nopeus)
		sprite.a = -max_nopeus;


	/*****************************************************************************************/
	/* Attacks 1 and 2                                                                       */
	/*****************************************************************************************/

	// If the sprite is ready and isn't crouching
	if (sprite.lataus == 0 && !sprite.kyykky)
	{
		// hy�kk�ysaika on "tapissa" mik� tarkoittaa sit�, ett� aloitetaan hy�kk�ys
		if (sprite.hyokkays1 == sprite.tyyppi->hyokkays1_aika)
		{
			// provides recovery time, after which the sprite can attack again
			sprite.lataus = sprite.tyyppi->latausaika;
			if(sprite.lataus == 0) sprite.lataus = 5;
			// jos spritelle ei ole m��ritelty omaa latausaikaa ...
			if (sprite.ammus1 > -1 && sprite.tyyppi->latausaika == 0)
			// ... ja ammukseen on, otetaan latausaika ammuksesta
				if (protot[sprite.ammus1].tulitauko > 0)
					sprite.lataus = protot[sprite.ammus1].tulitauko;

			// soitetaan hy�kk�ys��ni
			PK_Soita_Aani(sprite.tyyppi->aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			if (sprite.ammus1 > -1)
			{
				PK_Sprite_Lisaa_Ammus(protot[sprite.ammus1],0,sprite_x, sprite_y, i);

		//		if (protot[sprite.ammus1].aanet[AANI_HYOKKAYS1] > -1)
		//			PK_Soita_Aani(protot[sprite.ammus1].aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);
			}
		}

		// Sama kuin hy�kk�ys 1:ss�
		if (sprite.hyokkays2 == sprite.tyyppi->hyokkays2_aika)
		{
			sprite.lataus = sprite.tyyppi->latausaika;
			if(sprite.lataus == 0) sprite.lataus = 5;
			if (sprite.ammus2 > -1  && sprite.tyyppi->latausaika == 0)
				if (protot[sprite.ammus2].tulitauko > 0)
					sprite.lataus = protot[sprite.ammus2].tulitauko;

			PK_Soita_Aani(sprite.tyyppi->aanet[AANI_HYOKKAYS2],100,(int)sprite_x, (int)sprite_y,
						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			if (sprite.ammus2 > -1)
			{
				PK_Sprite_Lisaa_Ammus(protot[sprite.ammus2],0,sprite_x, sprite_y, i);

		//		if (protot[sprite.ammus2].aanet[AANI_HYOKKAYS1] > -1)
		//			PK_Soita_Aani(protot[sprite.ammus2].aanet[AANI_HYOKKAYS1],100, (int)sprite_x, (int)sprite_y,
		//						  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

			}
		}
	}

	// Random sounds
	if (sprite.tyyppi->aanet[AANI_RANDOM] != -1 && rand()%200 == 1 && sprite.energia > 0)
		PK_Soita_Aani(sprite.tyyppi->aanet[AANI_RANDOM],80,(int)sprite_x, (int)sprite_y,
					  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);


	// KEHITYSVAIHEEN APUTOIMINTOJA

	BYTE color;
	DWORD plk;

	if (PisteInput_Keydown(PI_B) && dev_mode)
	{

		if (i == pelaaja_index)
		{

			char lukua[50];
			itoa(palikat[1].yla,lukua,10);
			//gcvt(sprite_a,7,lukua);
			PisteDraw2_Font_Write(fontti1,lukua,310,50);

		}

		if (sprite.tyyppi->tiletarkistus)
			for (x=0;x<palikat_x_lkm;x++)
			{
				for (y=0;y<palikat_y_lkm;y++)
				{
					color = 50-x*2-y*2;
					plk = x+y*palikat_x_lkm;

					if (plk > 299)
						plk = 299;

					if (plk < 0)
						plk = 0;

					if (!palikat[plk].tausta)
						color += 32;

					PisteDraw2_ScreenFill(
											palikat[plk].vasen-kamera_x,
											palikat[plk].yla-kamera_y,
											palikat[plk].oikea-kamera_x,
											palikat[plk].ala-kamera_y,
											color);
				}
			}

		PisteDraw2_ScreenFill(
								(int)(sprite_vasen-kamera_x),
								(int)(sprite_yla-kamera_y),
								(int)(sprite_oikea-kamera_x),
								(int)(sprite_ala-kamera_y),
								30);

	}



	return 0;
}

int PK_Sprite_Liikuta_Bonus(int i){
	sprite_x = 0;
	sprite_y = 0;
	sprite_a = 0;
	sprite_b = 0;

	sprite_vasen = 0;
	sprite_oikea = 0;
	sprite_yla = 0;
	sprite_ala = 0;

	sprite_leveys  = 0;
	sprite_korkeus = 0;

	kartta_vasen = 0;
	kartta_yla   = 0;

	PK2Sprite &sprite = spritet[i];

	sprite_x = sprite.x;
	sprite_y = sprite.y;
	sprite_a = sprite.a;
	sprite_b = sprite.b;

	sprite_leveys  = sprite.tyyppi->leveys;
	sprite_korkeus = sprite.tyyppi->korkeus;

	x = 0;
	y = 0;
	oikealle	= true,
	vasemmalle	= true,
	ylos		= true,
	alas		= true;

	vedessa = sprite.vedessa;

	max_nopeus = (int)sprite.tyyppi->max_nopeus;

	// Siirret��n varsinaiset muuttujat apumuuttujiin.

	sprite_vasen = sprite_x-sprite_leveys/2;
	sprite_oikea = sprite_x+sprite_leveys/2;
	sprite_yla	 = sprite_y-sprite_korkeus/2;
	sprite_ala	 = sprite_y+sprite_korkeus/2;


	if (sprite.isku > 0)
		sprite.isku--;

	if (sprite.lataus > 0)
		sprite.lataus--;

	if (sprite.muutos_ajastin > 0)	// aika muutokseen
		sprite.muutos_ajastin --;

	// Hyppyyn liittyv�t seikat

	if (kytkin_tarina + jaristys > 0 && sprite.hyppy_ajastin == 0)
		sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy / 2;

	if (sprite.hyppy_ajastin > 0 && sprite.hyppy_ajastin < sprite.tyyppi->max_hyppy)
	{
		sprite.hyppy_ajastin ++;
		sprite_b = (sprite.tyyppi->max_hyppy - sprite.hyppy_ajastin)/-4.0;//-2
	}

	if (sprite_b > 0)
		sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy;



	if (sprite.paino != 0)	// jos bonuksella on paino, tutkitaan ymp�rist�
	{
		// o
		//
		// |  Gravity
		// V

		sprite_b += sprite.paino + sprite_b/1.25;

		if (sprite.vedessa)
		{
			if (sprite_b > 0)
				sprite_b /= 2.0;

			if (rand()%80 == 1)
				PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,sprite_x-4,sprite_y,0,-0.5-rand()%2,rand()%30+30,0,32);
		}

		sprite.vedessa = false;

		sprite.kytkinpaino = sprite.paino;

		/* TOISET SPRITET */

		PK2BLOCK spritepalikka;

		for (int sprite_index = 0; sprite_index < MAX_SPRITEJA; sprite_index++)
		{
			if (sprite_index != i && !spritet[sprite_index].piilota)
			{
				if (spritet[sprite_index].tyyppi->este && sprite.tyyppi->tiletarkistus)
				{
					if (sprite_x-sprite_leveys/2 +sprite_a <= spritet[sprite_index].x + spritet[sprite_index].tyyppi->leveys /2 &&
						sprite_x+sprite_leveys/2 +sprite_a >= spritet[sprite_index].x - spritet[sprite_index].tyyppi->leveys /2 &&
						sprite_y-sprite_korkeus/2+sprite_b <= spritet[sprite_index].y + spritet[sprite_index].tyyppi->korkeus/2 &&
						sprite_y+sprite_korkeus/2+sprite_b >= spritet[sprite_index].y - spritet[sprite_index].tyyppi->korkeus/2)
					{
						spritepalikka.koodi = 0;
						spritepalikka.ala   = (int)spritet[sprite_index].y + spritet[sprite_index].tyyppi->korkeus/2;
						spritepalikka.oikea = (int)spritet[sprite_index].x + spritet[sprite_index].tyyppi->leveys/2;
						spritepalikka.vasen = (int)spritet[sprite_index].x - spritet[sprite_index].tyyppi->leveys/2;
						spritepalikka.yla   = (int)spritet[sprite_index].y - spritet[sprite_index].tyyppi->korkeus/2;

						spritepalikka.alas       = BLOCK_SEINA;
						spritepalikka.ylos       = BLOCK_SEINA;
						spritepalikka.oikealle   = BLOCK_SEINA;
						spritepalikka.vasemmalle = BLOCK_SEINA;

						if (!spritet[sprite_index].tyyppi->este_alas)
							spritepalikka.alas		 = BLOCK_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_ylos)
							spritepalikka.ylos		 = BLOCK_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_oikealle)
							spritepalikka.oikealle   = BLOCK_TAUSTA;
						if (!spritet[sprite_index].tyyppi->este_vasemmalle)
							spritepalikka.vasemmalle = BLOCK_TAUSTA;


						spritepalikka.vesi  = false;

						PK_Palikka_Este(spritepalikka);
						PK_Tutki_Seina2(sprite, spritepalikka); //Colision bonus and sprite block
					}
				}

				if (sprite_x < spritet[sprite_index].x + spritet[sprite_index].tyyppi->leveys/2 &&
					sprite_x > spritet[sprite_index].x - spritet[sprite_index].tyyppi->leveys/2 &&
					sprite_y < spritet[sprite_index].y + spritet[sprite_index].tyyppi->korkeus/2 &&
					sprite_y > spritet[sprite_index].y - spritet[sprite_index].tyyppi->korkeus/2 &&
					sprite.isku == 0)
				{
					if (spritet[sprite_index].tyyppi->tyyppi != TYYPPI_BONUS &&
						!(sprite_index == pelaaja_index && sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU))
						sprite_a += spritet[sprite_index].a*(rand()%4);

					// lis�t��n spriten painoon sit� koskettavan toisen spriten paino
					sprite.kytkinpaino += spritet[sprite_index].tyyppi->paino;

					// samanmerkkiset spritet vaihtavat suuntaa t�rm�tess��n
					if (sprite.tyyppi->indeksi == spritet[sprite_index].tyyppi->indeksi &&
						spritet[sprite_index].energia > 0)
					{
						if (sprite.x < spritet[sprite_index].x)
						{
							spritet[sprite_index].a += sprite.a / 3.0;
							oikealle = false;
						}
						if (sprite.x > spritet[sprite_index].x)
						{
							spritet[sprite_index].a += sprite.a / 3.0;
							vasemmalle = false;
						}
						/*
						if (sprite.y < spritet[sprite_index].y)
							alas = false;
						if (sprite.y > spritet[sprite_index].y)
							ylos = false;*/
					}

				}
			}
		}

		// Tarkistetaan ettei menn� mihink��n suuntaan liian kovaa.

		if (sprite_b > 4)
			sprite_b = 4;

		if (sprite_b < -4)
			sprite_b = -4;

		if (sprite_a > 3)
			sprite_a = 3;

		if (sprite_a < -3)
			sprite_a = -3;

		// Lasketaan

		int palikat_x_lkm = 0,
			palikat_y_lkm = 0;

		if (sprite.tyyppi->tiletarkistus)
		{

			palikat_x_lkm = (int)((sprite_leveys) /32)+4;
			palikat_y_lkm = (int)((sprite_korkeus)/32)+4;

			kartta_vasen = (int)(sprite_vasen)/32;
			kartta_yla	 = (int)(sprite_yla)/32;

			for (y=0;y<palikat_y_lkm;y++)
				for (x=0;x<palikat_x_lkm;x++)
				{
					palikat[x+y*palikat_x_lkm] = PK_Palikka_Tutki(kartta_vasen+x-1,kartta_yla+y-1);
				}

			// Tutkitaan t�rm��k� palikkaan

			for (y=0;y<palikat_y_lkm;y++)
				for (x=0;x<palikat_x_lkm;x++)
					PK_Tutki_Seina(sprite, palikat[x+y*palikat_x_lkm]);
			/*
			PK_Tutki_Seina_Debug(sprite, palikat[x+y*palikat_x_lkm],
					sprite_x,
					sprite_y,
					sprite_a,
					sprite_b,
					sprite_vasen,
					sprite_oikea,
					sprite_yla,
					sprite_ala,
					sprite_leveys,
					sprite_korkeus,
					kartta_vasen,
					kartta_yla,
					oikealle,
					vasemmalle,
					ylos,
					alas);*/


		}

		if (vedessa != sprite.vedessa)
			PK_Tehoste_Loiskahdus((int)sprite_x,(int)sprite_y,32);


		if (!oikealle)
		{
			if (sprite_a > 0)
				sprite_a = -sprite_a/1.5;
		}

		if (!vasemmalle)
		{
			if (sprite_a < 0)
				sprite_a = -sprite_a/1.5;
		}

		if (!ylos)
		{
			if (sprite_b < 0)
				sprite_b = 0;

			sprite.hyppy_ajastin = sprite.tyyppi->max_hyppy;
		}

		if (!alas)
		{
			if (sprite_b >= 0)
			{
				if (sprite.hyppy_ajastin > 0)
				{
					sprite.hyppy_ajastin = 0;
				//	if (/*sprite_b == 4*/!maassa)
				//		PK_Soita_Aani(tomahdys_aani,20,(int)sprite_x, (int)sprite_y,
				//				      int(25050-sprite.tyyppi->paino*4000),true);
				}

				if (sprite_b > 2)
					sprite_b = -sprite_b/(3+rand()%2);
				else
					sprite_b = 0;
			}
			//sprite_a /= kitka;
			sprite_a /= 1.07;
		}
		else
		{
			sprite_a /= 1.02;
		}

		sprite_b /= 1.5;

		if (sprite_b > 4)
			sprite_b = 4;

		if (sprite_b < -4)
			sprite_b = -4;

		if (sprite_a > 4)
			sprite_a = 4;

		if (sprite_a < -4)
			sprite_a = -4;

		sprite_x += sprite_a;
		sprite_y += sprite_b;

		sprite.x = sprite_x;
		sprite.y = sprite_y;
		sprite.a = sprite_a;
		sprite.b = sprite_b;

		sprite.oikealle = oikealle;
		sprite.vasemmalle = vasemmalle;
		sprite.alas = alas;
		sprite.ylos = ylos;
	}
	else	// jos spriten paino on nolla, tehd��n spritest� "kelluva"
	{
		sprite.y = sprite.alku_y + cos_table[int(degree+(sprite.alku_x+sprite.alku_y))%360] / 3.0;
		sprite_y = sprite.y;
	}

	sprite.paino = sprite.alkupaino;

	int tuhoutuminen;

	// Test if player touches bonus
	if (sprite_x < spritet[pelaaja_index].x + spritet[pelaaja_index].tyyppi->leveys/2 &&
		sprite_x > spritet[pelaaja_index].x - spritet[pelaaja_index].tyyppi->leveys/2 &&
		sprite_y < spritet[pelaaja_index].y + spritet[pelaaja_index].tyyppi->korkeus/2 &&
		sprite_y > spritet[pelaaja_index].y - spritet[pelaaja_index].tyyppi->korkeus/2 &&
		sprite.isku == 0)
	{
		if (sprite.energia > 0 && spritet[pelaaja_index].energia > 0)
		{
			if (sprite.tyyppi->pisteet != 0)
			{
				piste_lisays += sprite.tyyppi->pisteet;
				char luku[6];
				itoa(sprite.tyyppi->pisteet,luku,10);
				if (sprite.tyyppi->pisteet >= 50)
					PK_Fadeteksti_Uusi(fontti2,luku,(int)sprite.x-8,(int)sprite.y-8,100);
				else
					PK_Fadeteksti_Uusi(fontti1,luku,(int)sprite.x-8,(int)sprite.y-8,100);

			}

			if (sprite.Onko_AI(AI_BONUS_AIKA))
				aika += sprite.tyyppi->latausaika;

			if (sprite.Onko_AI(AI_BONUS_NAKYMATTOMYYS))
				nakymattomyys = sprite.tyyppi->latausaika;

			//kartta->spritet[(int)(sprite.alku_x/32) + (int)(sprite.alku_y/32)*PK2KARTTA_KARTTA_LEVEYS] = 255;

			if (sprite.tyyppi->vahinko != 0 && sprite.tyyppi->tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
				spritet[pelaaja_index].energia -= sprite.tyyppi->vahinko;

			tuhoutuminen = sprite.tyyppi->tuhoutuminen;

			if (tuhoutuminen != TUHOUTUMINEN_EI_TUHOUDU)
			{
				if (tuhoutuminen >= TUHOUTUMINEN_ANIMAATIO)
					tuhoutuminen -= TUHOUTUMINEN_ANIMAATIO;
				else
				{
					if (sprite.tyyppi->avain)
					{
						avaimia--;

						if (avaimia < 1)
							PK_Kartta_Avaa_Lukot();
					}

					sprite.piilota = true;
				}

				if (sprite.Onko_AI(AI_UUSI_JOS_TUHOUTUU)) {
					double ax, ay;
					ax = sprite.alku_x;//-sprite.tyyppi->leveys;
					ay = sprite.alku_y-sprite.tyyppi->korkeus/2.0;
					PK_Sprite_Lisaa(*sprite.tyyppi,0,ax-17, ay,i);
					for (int r=1;r<6;r++)
						PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,ax+rand()%10-rand()%10, ay+rand()%10-rand()%10,0,0,rand()%100,0.1,32);

				}

				if (sprite.tyyppi->bonus  != -1)
					PK_Esineet_Lisaa(&protot[sprite.tyyppi->bonus]);

				if (sprite.tyyppi->muutos != -1)
				{
					if (protot[sprite.tyyppi->muutos].AI[0] != AI_BONUS)
					{
						spritet[pelaaja_index].tyyppi = &protot[sprite.tyyppi->muutos];
						spritet[pelaaja_index].ammus1 = spritet[pelaaja_index].tyyppi->ammus1;
						spritet[pelaaja_index].ammus2 = spritet[pelaaja_index].tyyppi->ammus2;
						spritet[pelaaja_index].alkupaino = spritet[pelaaja_index].tyyppi->paino;
						spritet[pelaaja_index].y -= spritet[pelaaja_index].tyyppi->korkeus/2;
						//PK_Uusi_Ilmoitus("pekka has been transformed!");
					}
				}

				if (sprite.tyyppi->ammus1 != -1)
				{
					spritet[pelaaja_index].ammus1 = sprite.tyyppi->ammus1;
					PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_newegg));
				}

				if (sprite.tyyppi->ammus2 != -1)
				{
					spritet[pelaaja_index].ammus2 = sprite.tyyppi->ammus2;
					PK_Uusi_Ilmoitus(tekstit->Hae_Teksti(txt_game_newdoodle));
				}

				PK_Soita_Aani(sprite.tyyppi->aanet[AANI_TUHOUTUMINEN],100, (int)sprite.x, (int)sprite.y,
							  sprite.tyyppi->aani_frq, sprite.tyyppi->random_frq);

				PK_Tehosteet(tuhoutuminen, (DWORD)sprite_x, (DWORD)sprite_y);
			}
		}
	}

	for (i=0;i<SPRITE_MAX_AI;i++)
	{
		if (sprite.tyyppi->AI[i] == AI_EI)
			break;

		switch (sprite.tyyppi->AI[i])
		{
		case AI_BONUS:				sprite.AI_Bonus();break;

		case AI_PERUS:				sprite.AI_Perus();break;

		case AI_MUUTOS_AJASTIN:		if (sprite.tyyppi->muutos > -1)
									sprite.AI_Muutos_Ajastin(protot[sprite.tyyppi->muutos]);
									break;

		case AI_TIPPUU_TARINASTA:	sprite.AI_Tippuu_Tarinasta(jaristys + kytkin_tarina);
									break;

		default:					break;
		}
	}

	/* Ei k�y p�ins�, ett� pelaaja on bonusesine */
	if (sprite.pelaaja != 0)
		sprite.energia = 0;

	return 0;
}

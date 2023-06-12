/*
NIST-developed software is provided by NIST as a public service. You may use, copy, and distribute copies of the software in any medium, provided that you keep intact this entire notice. You may improve, modify, and create derivative works of the software or any portion of the software, and you may copy and distribute such modifications or works. Modified works should carry a notice stating that you changed the software and should note the date and nature of any such change. Please explicitly acknowledge the National Institute of Standards and Technology as the source of the software.
 
NIST-developed software is expressly provided "AS IS." NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT, OR ARISING BY OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.
 
You are solely responsible for determining the appropriateness of using and distributing the software and you assume all risks associated with its use, including but not limited to the risks and costs of program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and the unavailability or interruption of operation. This software is not intended to be used in any situation where a failure could cause risk of injury or damage to property. The software developed by NIST employees is not subject to copyright protection within the United States.
*/

#ifndef API_H
#define API_H

//  Set these three values apropriately for your algorithm
// PALOMA1 
#define PALOMA1_SECRETKEYBYTES       93008
#define PALOMA1_PUBLICKEYBYTES       319488 
#define PALOMA1_CRYPTO_BYTES         32
#define PALOMA1_CIPHERTEXTBYTES      136 
// PALOMA2
#define PALOMA2_SECRETKEYBYTES       355400
#define PALOMA2_PUBLICKEYBYTES       812032 
#define PALOMA2_CRYPTO_BYTES         32
#define PALOMA2_CIPHERTEXTBYTES      240 
// PALOMA3
#define PALOMA3_SECRETKEYBYTES       357064
#define PALOMA3_PUBLICKEYBYTES       1025024 
#define PALOMA3_CRYPTO_BYTES         32
#define PALOMA3_CIPHERTEXTBYTES      240 


// Change the algorithm name
#define CRYPTO_ALGNAME "PALOMA"

#include "gf.h"
int crypto_PALOMA_keypair(unsigned char* pk, unsigned char* sk, PARAM* PALOMAParam);
int crypto_PALOMA_enc(unsigned char* ct, unsigned char* key, const unsigned char* pk, PARAM* PALOMAParam);
int crypto_PALOMA_dec(unsigned char* key, const unsigned char* ct, const unsigned char* sk, PARAM* PALOMAParam);



#endif /* api_h */

/*
 * Jenkins.h
 *
 *  Created on: 17/11/2014
 *      Author: hernan
 */

#ifndef FIUBA_DATOS_HASHING_JENKINS_H_
#define FIUBA_DATOS_HASHING_JENKINS_H_

#include <iostream>
#include "stdint.h"

class Jenkins {
public:
	Jenkins();

	uint32_t hashearCon32Bits(std::string nombre);
	uint64_t hashearCon64Bits(std::string nombre);
	virtual ~Jenkins();
};

#endif /* FIUBA_DATOS_HASHING_JENKINS_H_ */

/* Expectation Maximization for Gaussian Mixture Models.
Copyright (C) 2012-2014 Juan Daniel Valor Miro

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details. */

#ifndef _workers_h
#define _workers_h

	typedef workers,workers_mutex; /* Data types of the workers. */

	/* Public function prototypes to work with the workers pool. */
	workers *workers_create(number);
	void workers_addtask(workers*,void(*)(void*),void*);
	void workers_waitall(workers*);
	void workers_finish(workers*);
	number workers_number(workers*);

	/* Public function prototypes to work with the workers mutex. */
	workers_mutex *workers_mutex_create();
	void workers_mutex_delete(workers_mutex*);
	void workers_mutex_lock(workers_mutex*);
	void workers_mutex_unlock(workers_mutex*);

#endif

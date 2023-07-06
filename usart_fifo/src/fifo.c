/*****************************************************************************
 
 fifo.c - FIFO library 
 
 Copyright (C) 2015 Falk Brunner 
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 
 You can contact the author at Falk.Brunner@gmx.de
 
*****************************************************************************/

/*
	Version 1.00
	
*/

#include <stdint.h>
#include <util/atomic.h>
#include "fifo.h"

void fifo_write_busy(fifo_t *fifo, fifo_data_t data) {

    while(!fifo_get_free(fifo));
    fifo_write(fifo, data);
}

void fifo_write(fifo_t *fifo, fifo_data_t data) {
    fifo_data_t  *tmp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = (fifo_data_t*)fifo->write_p;
        *tmp++ = data;
        if (tmp > fifo->top) {
            fifo->write_p = fifo->base;
        } else {
            fifo->write_p = tmp;
        }
    }
}

fifo_data_t fifo_read_busy(fifo_t *fifo) {

    while(!fifo_get_level(fifo));
    return fifo_read(fifo);
}

fifo_data_t fifo_read(fifo_t *fifo) {
    fifo_data_t data;
    fifo_data_t *tmp;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = (fifo_data_t*)fifo->read_p;
        data = *tmp++;
        if (tmp > fifo->top) {
            fifo->read_p = fifo->base;
        } else {
            fifo->read_p = tmp;
        }
    }
    return data;
}

fifo_size_t fifo_get_level(fifo_t *fifo) {
    fifo_size_t tmp; 
    fifo_data_t *tmp_w, *tmp_r;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp_w = (fifo_data_t*)fifo->write_p;
        tmp_r = (fifo_data_t*)fifo->read_p;
    }
    tmp = tmp_w - tmp_r;
    if (tmp_w < tmp_r) {
        tmp += fifo->size;
    } 
    return tmp;
}

fifo_size_t fifo_get_free(fifo_t *fifo) {
    fifo_size_t tmp;
    fifo_data_t *tmp_w, *tmp_r;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp_w = (fifo_data_t*)fifo->write_p;
        tmp_r = (fifo_data_t*)fifo->read_p;
    }
    tmp = tmp_w - tmp_r;
    if (tmp_w < tmp_r) {
        tmp += fifo->size;
    }
    tmp =  fifo->size - tmp - 1;
    return tmp;
}

// correct fifo write pointer after burst write
// burst sizes that lead beyond the FIFO data block are truncated

void fifo_write_bursted(fifo_t *fifo, fifo_size_t count) {
    fifo_data_t* tmp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = (fifo_data_t*)(fifo->write_p + count);
        if (tmp > fifo->top) {
            fifo->write_p = fifo->base;
        } else {
            fifo->write_p = tmp;
        }
    }
}
 
// correct fifo read pointer after burst read
// burst sizes that lead beyond the FIFO data block are truncated

void fifo_read_bursted(fifo_t *fifo, fifo_size_t count) {
    fifo_data_t* tmp;    

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = (fifo_data_t*)fifo->read_p + count;

        if (tmp > fifo->top) {
            fifo->read_p = fifo->base;
        } else {
            fifo->read_p = tmp;
        }
    }
}

// number of elements to write before pointer wraps around

fifo_size_t fifo_get_write_wrap(fifo_t *fifo) {
    fifo_size_t tmp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = fifo->top - fifo->write_p + 1;
    }
    return tmp;
}

// number of elements to read before pointer wraps around

fifo_size_t fifo_get_read_wrap(fifo_t *fifo) {
    fifo_size_t tmp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = fifo->top - fifo->read_p + 1;
    }
    return tmp;
}

void fifo_init(fifo_t *fifo, fifo_data_t *data, fifo_size_t size) {
    fifo->write_p = data;
    fifo->read_p  = data;
    fifo->base    = data;
    fifo->top     = data + size - 1;
    fifo->size    = size;
}

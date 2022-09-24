/*****************************************************************************
 
 Software FIFO using circular buffer
 structs and generic functions to increase usability
 
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
 
/**
\mainpage

 \par fifo.h - FIFO library

 \author Falk Brunner

 \version 1.00
 
 \par License:
 \subpage LICENSE "GNU Lesser General Public License"
 
 \par Files:
	\subpage FIFO.H \n
	\subpage FIFO.C

 \par Developed on AVR plattform, but can be used with any other
      AVR-Studio 4.18, WinAVR20100110 (avr-gcc 4.3.3) \n
      Atomic access must be adapted to other plattforms!

 \par Example:
 \include ./main_fifo.c
 \page LICENSE GNU Lesser General Public License
 \include ./lgpl.txt
 \page FIFO.H fifo.h
 \include ./fifo.h
 \page FIFO.C fifo.c
 \include ./fifo.c
*/

#ifndef _FIFO_H
#define _FIFO_H

/** \defgroup FIFO_CONFIGURATION FIFO CONFIGURATION
 Define the data type for your FIFO data \n
 Define the size of your FIFO size\n
*/
/*@{*/

typedef char  fifo_data_t;       /**< FIFO data element type */
typedef uint8_t fifo_size_t;       /**< FIFO size type */

/*@}*/

/*! \struct fifo_t fifo.h "fifo.h"
 *  \brief FIFO control data struct
 */

typedef struct {
    fifo_data_t volatile * volatile write_p; /**< write pointer */
    fifo_data_t volatile * volatile read_p;  /**< read pointer */
    fifo_data_t volatile * volatile base;    /**< base pointer */
    fifo_data_t volatile * volatile top;     /**< top pointer */
    fifo_size_t volatile size;               /**< size */
} fifo_t;

//-FUNCTIONS---------------------------------------------------------------------------------------------------------
/** \defgroup FUNCTIONS FUNCTIONS 
  * Standard functions, can be used in normal programm or ISR.
  */
/*@{*/ 

/**
 \brief FIFO initialization
 \brief FIFO can hold only size-1 elements! One element is unused due to organization. 
 \param *fifo pointer to FIFO struct
 \param *data pointer to data buffer
 \param size size of data buffer in elements
 \return none
 */    
void fifo_init(fifo_t *fifo, fifo_data_t *data, fifo_size_t size);

/**
 \brief FIFO write access without check for free space
 \param *fifo pointer to FIFO struct
 \param data write data
 \return none
 */
void fifo_write(fifo_t *fifo, fifo_data_t data);

/**
 \brief FIFO write access with busy waiting for free space (blocking function)
 \param *fifo pointer to FIFO struct
 \param data write data
 \return none
 */
void fifo_write_busy(fifo_t *fifo, fifo_data_t data);

/**
 \brief FIFO read access with busy waiting for available data (blocking function)
 \param *fifo pointer to FIFO struct
 \return read data
 */
fifo_data_t fifo_read_busy(fifo_t *fifo);

/**
 \brief FIFO read access without check for available data
 \param *fifo pointer to FIFO struct
 \return read data
 */
fifo_data_t fifo_read(fifo_t *fifo);

/**
 \brief Get fill level of FIFO
 \param *fifo pointer to FIFO struct
 \return fill level of FIFO in elements
 */
fifo_size_t fifo_get_level(fifo_t *fifo);

/**
 \brief Get free space of fifo
 \param *fifo pointer to FIFO struct
 \return free space of FIFO in elements
 */
fifo_size_t fifo_get_free(fifo_t *fifo);

/**
 \brief FIFO write pointer correction after burst write access
 \param *fifo pointer to FIFO struct
 \param count number of elements that have been written
 \return none
 */
void fifo_write_bursted(fifo_t *fifo, fifo_size_t count);

/**
 \brief FIFO read pointer correction after burst read access
 \param *fifo pointer to FIFO struct
 \param count number of elements that have been read
 \return none
 */
void fifo_read_bursted(fifo_t *fifo, fifo_size_t count);

/**
 \brief Get number of elements for write access until pointer wrap around
 \param *fifo pointer to FIFO struct
 \return number of elements for write access until pointer wrap around
 */
fifo_size_t fifo_get_write_wrap(fifo_t *fifo);

/**
 \brief Get number of elements for read access until pointer wrap around
 \param *fifo pointer to FIFO struct
 \return number of elements for read access until pointer wrap around
 */
fifo_size_t fifo_get_read_wrap(fifo_t *fifo);

/*@}*/

//-ISR FUNCTIONS---------------------------------------------------------------------------------------------------------
/** \defgroup ISR_FUNCTIONS ISR FUNCTIONS 
  * Inline functions for fast FIFO access in ISR. Use in ISR only!
  */
/*@{*/ 

/**
 \brief FIFO write access without check for free space
 \param *fifo pointer to FIFO struct
 \param data write data
 \return none
 */
static inline void fifo_write_ISR(fifo_t *fifo, fifo_data_t data);

/**
 \brief FIFO read access without check for available data
 \param *fifo pointer to FIFO struct
 \return read data
 */
static inline fifo_data_t fifo_read_ISR(fifo_t *fifo);

/**
 \brief Get fill level of FIFO
 \param *fifo pointer to FIFO struct
 \return fill level of fifo in elements
 */
static inline fifo_size_t fifo_get_level_ISR(fifo_t *fifo);

/**
 \brief Get free space of fifo
 \param *fifo pointer to FIFO struct
 \return free space of fifo in elements
 */
static inline fifo_size_t fifo_get_free_ISR(fifo_t *fifo);

/*@}*/ 

//- INLINE FUNCTIONS ------------------------------------------------------------------------------------------------------------------

static inline fifo_data_t fifo_read_ISR(fifo_t *fifo) {                                                       
    fifo_data_t data;                                       
    fifo_data_t *tmp;                                       
                                                        
    tmp = (fifo_data_t*)fifo->read_p;                                
    data = *tmp++;                                      
    if (tmp > fifo->top) {   
         fifo->read_p = fifo->base;         
    } else {                                            
        fifo->read_p = tmp;                            
    }                                                   
    return data;
}

static inline void fifo_write_ISR(fifo_t *fifo, fifo_data_t data) {                                                       
    fifo_data_t *tmp;                                       
                                                        
    tmp = (fifo_data_t*)fifo->write_p;                               
    *tmp++ = data;                               
    if (tmp > fifo->top) {   
        fifo->write_p = fifo->base;         
    } else {                                            
        fifo->write_p = tmp;                           
    }                                                   
}
        
static inline fifo_size_t fifo_get_level_ISR(fifo_t *fifo) {                                                       
    fifo_size_t tmp;                                       
    fifo_data_t *tmp_w, *tmp_r;                             
                                                        
    tmp_w = (fifo_data_t*)fifo->write_p;                             
    tmp_r = (fifo_data_t*)fifo->read_p;                              
    
    tmp = tmp_w - tmp_r;                                
    if (tmp_w < tmp_r) {                                
        tmp += fifo->size;
    }                                                  
    return tmp;                                   
}

static inline fifo_size_t fifo_get_free_ISR(fifo_t *fifo) {                                                       
    fifo_size_t tmp;                                       
    fifo_data_t *tmp_w, *tmp_r;                             
                                                        
    tmp_w = (fifo_data_t*)fifo->write_p;                             
    tmp_r = (fifo_data_t*)fifo->read_p;                              
    tmp = tmp_w - tmp_r;                                
    if (tmp_w < tmp_r) {                                
        tmp += fifo->size;                             
    }                                                   
    return (fifo->size - tmp - 1);                      
}

#endif

/*****************************************************************************
 * raw.c: x264 raw bitstream output module
 *****************************************************************************
 * Copyright (C) 2003-2009 x264 project
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Loren Merritt <lorenm@u.washington.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *****************************************************************************/

#include "muxers.h"

static int open_file( char *psz_filename, hnd_t *p_handle )
{
    if( !strcmp( psz_filename, "-" ) )
        *p_handle = stdout;
    else if( !(*p_handle = fopen( psz_filename, "w+b" )) )
        return -1;

    return 0;
}

static int set_param( hnd_t handle, x264_param_t *p_param )
{
    return 0;
}

static int write_nalu( hnd_t handle, uint8_t *p_nalu, int i_size, x264_picture_t *p_picture )
{
    if( fwrite( p_nalu, i_size, 1, (FILE*)handle ) > 0 )
        return i_size;
    return -1;
}

static int set_eop( hnd_t handle, x264_picture_t *p_picture )
{
    return 0;
}

static int close_file( hnd_t handle )
{
    if( !handle || handle == stdout )
        return 0;

    return fclose( (FILE*)handle );
}

cli_output_t raw_output = { open_file, set_param, write_nalu, set_eop, close_file };

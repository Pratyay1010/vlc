/*****************************************************************************
 * vlc_es.h
 *****************************************************************************
 * Copyright (C) 1999-2001 VideoLAN
 * $Id: vlc_es.h,v 1.4 2003/11/24 00:39:00 fenrir Exp $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

#ifndef _VLC_ES_H
#define _VLC_ES_H 1

/**
 * Description of a audio frame
 */
struct audio_format_t
{
    vlc_fourcc_t        i_format;

    unsigned int        i_rate;

    /* Describes the channels configuration of the samples (ie. number of
     * channels which are available in the buffer, and positions). */
    uint32_t            i_physical_channels;

    /* Describes from which original channels, before downmixing, the
     * buffer is derived. */
    uint32_t            i_original_channels;

    /* Optional - for A/52, SPDIF and DTS types : */
    /* Bytes used by one compressed frame, depends on bitrate. */
    unsigned int        i_bytes_per_frame;

    /* Number of sampleframes contained in one compressed frame. */
    unsigned int        i_frame_length;
    /* Please note that it may be completely arbitrary - buffers are not
     * obliged to contain a integral number of so-called "frames". It's
     * just here for the division :
     * buffer_size = i_nb_samples * i_bytes_per_frame / i_frame_length
     */

    /* FIXME ? (used by the codecs) */
    int i_channels;
    int i_blockalign;
    int i_bitspersample;
};

/**
 * Description of a video frame
 */
struct video_format_t
{
    vlc_fourcc_t i_chroma;                               /**< picture chroma */
    unsigned int i_aspect;                                 /**< aspect ratio */

    unsigned int i_width;                                 /**< picture width */
    unsigned int i_height;                               /**< picture height */
    unsigned int i_x_offset;               /**< start offset of visible area */
    unsigned int i_y_offset;               /**< start offset of visible area */
    unsigned int i_visible_width;                 /**< width of visible area */
    unsigned int i_visible_height;               /**< height of visible area */

    unsigned int i_bits_per_pixel;             /**< number of bits per pixel */

    unsigned int i_frame_rate;                     /**< frame rate numerator */
    unsigned int i_frame_rate_base;              /**< frame rate denominator */
};

/**
 * Description of subs
 */
struct subs_format_t
{
    char *psz_encoding;

    struct
    {
        /* FIXME */
        uint32_t palette[16+1];
    } spu;
};

/**
 * ES definition
 */
struct es_format_t
{
    int             i_cat;
    vlc_fourcc_t    i_codec;

    int             i_group;    /* -1 : standalone
                                   >= 0 then a "group" (program) is created
                                        for each value */
    int             i_priority; /*  -2 : mean not selectable by the users
                                    -1 : mean not selected by default even
                                        when no other stream
                                    >=0: priority */
    char            *psz_language;
    char            *psz_description;

    audio_format_t audio;
    video_format_t video;
    subs_format_t  subs;

    int     i_bitrate;

    int     i_extra;
    void    *p_extra;

};

/* ES Categories */
#define UNKNOWN_ES      0x00
#define VIDEO_ES        0x01
#define AUDIO_ES        0x02
#define SPU_ES          0x03
#define NAV_ES          0x04

static inline void es_format_Init( es_format_t *fmt,
                                   int i_cat, vlc_fourcc_t i_codec )
{
    fmt->i_cat                  = i_cat;
    fmt->i_codec                = i_codec;
    fmt->i_group                = 0;
    fmt->i_priority             = 0;
    fmt->psz_language           = NULL;
    fmt->psz_description        = NULL;

    memset( &fmt->audio, 0, sizeof(audio_format_t) );
    memset( &fmt->video, 0, sizeof(video_format_t) );
    memset( &fmt->subs, 0, sizeof(subs_format_t) );

    fmt->i_bitrate              = 0;
    fmt->i_extra                = 0;
    fmt->p_extra                = NULL;
}

static inline void es_format_Copy( es_format_t *dst, es_format_t *src )
{
    memcpy( dst, src, sizeof( es_format_t ) );
    if( src->i_extra > 0 )
    {
        dst->p_extra = malloc( src->i_extra );
        memcpy( dst->p_extra, src->p_extra,
                src->i_extra );
    }
    else
    {
        dst->i_extra = 0;
        dst->p_extra = NULL;
    }
}

#endif


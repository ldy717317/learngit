#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#define PI 3.14

snd_pcm_t *open_sound_dev(snd_pcm_stream_t type)
{
	int err;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *hw_params;
	unsigned int rate = 44000;

	if ((err = snd_pcm_open (&handle, "default", type, 0)) < 0) {
		return NULL;
	}
	   
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		return NULL;
	}
			 
	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &rate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, 1)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		return NULL;
	}

	if ((err = snd_pcm_hw_params (handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		return NULL;
	}

	snd_pcm_hw_params_free (hw_params);

	return handle;  
}

void close_sound_dev(snd_pcm_t *handle)
{
	snd_pcm_close (handle);
}

snd_pcm_t *open_playback(void)
{
    return open_sound_dev(SND_PCM_STREAM_PLAYBACK);
}

snd_pcm_t *open_capture(void)
{
    return open_sound_dev(SND_PCM_STREAM_CAPTURE);
}

             int i =0;
int main (int argc, char *argv[])
{
	int err;
	signed short buf[100];
	snd_pcm_t *playback_handle;
	snd_pcm_t *capture_handle;


    playback_handle = open_playback();
    if (!playback_handle)
    {
		fprintf (stderr, "cannot open for playback\n");
        return -1;
    }


    capture_handle = open_capture();
    if (!capture_handle)
    {
		fprintf (stderr, "cannot open for capture\n");
        return -1;
    }

	if ((err = snd_pcm_prepare (playback_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_prepare (capture_handle)) < 0)
	  {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		return -1;
	  }
	                      
	                        double  j =0.0;
	            	        j = 2.0*PI/440.0;
	            	        signed short  buf1[100];
	            	        float   k;
	            	        for(i=0,4=0.0;i<100,k<440.0;i++,k=k+4.4)
		                 {           
		                          buf1[i] =(signed short)((sin(j*k))*32768.0);
		                          printf("%f %f %d\n",sin(j*i),((sin(j*k))*32768.0),buf1[i]);
	
		                 }
                  
	                       while (1)
	                      {
	            	        for(i=0;i<100;i++)
		                 {
		                   buf[i] =buf1[i];                 
				   //buf[i] = 0xff;
		                 }
                                 err = snd_pcm_writei(playback_handle, buf, 100);
                                 printf("err = %d\n",err);
		                 if (err  != 100)         
		                 {
			              
				    return -1;
			         }
		       
	       		     }

				snd_pcm_close (playback_handle);
				//snd_pcm_close (capture_handle);
   				 return 0;
}


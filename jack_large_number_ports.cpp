/** @file large_number_ports.cpp
 *
 * @brief This simple client opens a large number of input
 * and output ports and attempts to connect its input from
 * a previous clients outputs.
 * The goal is to stress the port handling code within the
 * jack server.
 */

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>

#include <jack/jack.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::unique_ptr;
using std::vector;
using std::make_unique;
using std::stringstream;

jack_client_t * jack_client;
constexpr const uint32_t num_ports = 1024;
vector<jack_port_t*> input_ports;
vector<jack_port_t*> output_ports;

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
 * the user (e.g. using Ctrl-C on a unix-ish operating system)
 */
int process (jack_nframes_t nframes, void *arg)
{
    jack_default_audio_sample_t *in, *out;

    for( uint32_t port = 0 ; port < num_ports ; ++port )
    {
	in = (jack_default_audio_sample_t*)jack_port_get_buffer( input_ports[port], nframes );
	out = (jack_default_audio_sample_t*)jack_port_get_buffer( output_ports[port], nframes );
	std::copy( &in[0], &in[nframes], &out[0] );
    }

    return 0;      
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown (void *arg)
{
    exit (1);
}

int main (int argc, char *argv[])
{
    const char **ports;
    const char *client_name = "jack_large_number_ports";
    const char *server_name = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;
	
    /* open a client connection to the JACK server */
    jack_client = jack_client_open( client_name, options, &status, server_name );

    if( jack_client == NULL)
    {
	fprintf (stderr, "jack_client_open() failed, status = 0x%2.0x\n", status);
	if( status & JackServerFailed )
	{
	    fprintf (stderr, "Unable to connect to JACK server\n");
	}
	exit(1);
    }
    if( status & JackServerStarted )
    {
	fprintf (stderr, "JACK server started\n");
    }
    if( status & JackNameNotUnique )
    {
	client_name = jack_get_client_name( jack_client );
	fprintf( stderr, "unique name `%s' assigned\n", client_name );
    }

    /* tell the JACK server to call `process()' whenever
       there is work to be done.
    */

    jack_set_process_callback( jack_client, process, 0 );

    /* tell the JACK server to call `jack_shutdown()' if
       it ever shuts down, either entirely, or if it
       just decides to stop calling us.
    */

    jack_on_shutdown( jack_client, jack_shutdown, 0 );

    /* display the current sample rate. 
     */

    printf( "engine sample rate: %" PRIu32 "\n",
	    jack_get_sample_rate( jack_client ) );

    /* create our ports */
    for( uint32_t i = 0 ; i < num_ports ; ++i )
    {
	stringstream ss( stringstream::out );
	ss << "input-" << i;
	string port_name( ss.str() );
	jack_port_t * tmp_port = jack_port_register( jack_client,
						     port_name.c_str(),
						     JACK_DEFAULT_AUDIO_TYPE,
						     JackPortIsInput,
						     0 );
	if( tmp_port == nullptr )
	{
	    cerr << "Failed to register input port " << i << endl;
	    exit(1);
	}
	input_ports.push_back( tmp_port );
    }

    for( uint32_t o = 0 ; o < num_ports ; ++o )
    {
	stringstream ss( stringstream::out );
	ss << "output-" << o;
	string port_name( ss.str() );
	jack_port_t * tmp_port = jack_port_register( jack_client,
						     port_name.c_str(),
						     JACK_DEFAULT_AUDIO_TYPE,
						     JackPortIsOutput,
						     0 );
	if( tmp_port == nullptr )
	{
	    cerr << "Failed to register output port " << o << endl;
	    exit(1);
	}
	output_ports.push_back( tmp_port );
    }


    /* Tell the JACK server that we are ready to roll.  Our
     * process() callback will start running now. */

    if( jack_activate( jack_client ) )
    {
	fprintf( stderr, "cannot activate client" );
	exit(1);
    }

    /* Connect the ports.  You can't do this before the client is
     * activated, because we can't make connections to clients
     * that aren't running.  Note the confusing (but necessary)
     * orientation of the driver backend ports: playback ports are
     * "input" to the backend, and capture ports are "output" from
     * it.
     */

    /*
    ports = jack_get_ports( jack_client, NULL, NULL, JackPortIsPhysical|JackPortIsOutput );
    if( ports == NULL )
    {
	fprintf( stderr, "no physical capture ports\n" );
	exit(1);
    }

    if( jack_connect( jack_client, ports[0], jack_port_name( input_port ) )) {
	fprintf( stderr, "cannot connect input ports\n" );
    }

    free( ports );
	
    ports = jack_get_ports( jack_client, NULL, NULL, JackPortIsPhysical|JackPortIsInput );
    if( ports == NULL ) {
	fprintf( stderr, "no physical playback ports\n" );
	exit(1);
    }

    if( jack_connect( jack_client, jack_port_name( output_port ), ports[0]) ) {
	fprintf( stderr, "cannot connect output ports\n" );
    }

    free( ports );
    */

    /* keep running until stopped by the user */

    std::this_thread::sleep_for( std::chrono::seconds(60) );

    /* this is never reached but if the program
       had some other way to exit besides being killed,
       they would be important to call.
    */
    cout << "Starting close" << endl;

    jack_deactivate( jack_client );

    jack_client_close( jack_client );
    exit( 0 );
}

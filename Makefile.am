MAINTAINERCLEANFILES    = Makefile.in

if HAVE_SNDFILE
JACKREC = jack_rec
dist-check-sndfile:
else
JACKREC =
dist-check-sndfile:
	@echo
	@echo ' ******' You need sndfile installed to make dist.' ******'
	@echo
	@false
endif

bin_PROGRAMS = jack_simple_client \
	       jack_simple_session_client \
	       jack_transport_client \
	       jack_impulse_grabber \
	       jack_metro \
	       jack_showtime \
	       jack_midisine \
	       jack_midiseq \
	       jack_latent_client \
	       jack_server_control \
	       jack_large_number_ports \
	       $(JACKREC)

if HAVE_SNDFILE
# note! jackrec_CFLAGS syntax not supported by automake-1.4
sndfile_cflags = @SNDFILE_CFLAGS@
endif

AM_CFLAGS = -I.. $(JACK_CFLAGS) $(sndfile_cflags)
AM_CXXFLAGS = -I.. $(JACK_CPPFLAGS) $(sndfile_cflags)

jack_simple_client_SOURCES = simple_client.c
jack_simple_client_LDFLAGS = @OS_LDFLAGS@
jack_simple_client_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_simple_session_client_SOURCES = simple_session_client.c
jack_simple_session_client_LDFLAGS = @OS_LDFLAGS@
jack_simple_session_client_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_transport_client_SOURCES = transport_client.c
jack_transport_client_LDFLAGS = @OS_LDFLAGS@
jack_transport_client_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_metro_SOURCES = metro.c
jack_metro_LDFLAGS = @OS_LDFLAGS@
jack_metro_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_showtime_SOURCES = showtime.c
jack_showtime_LDFLAGS = @OS_LDFLAGS@
jack_showtime_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_impulse_grabber_SOURCES = impulse_grabber.c
jack_impulse_grabber_LDFLAGS = @OS_LDFLAGS@
jack_impulse_grabber_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_midiseq_SOURCES = midiseq.c
jack_midiseq_LDFLAGS = @OS_LDFLAGS@
jack_midiseq_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_midisine_SOURCES = midisine.c
jack_midisine_LDFLAGS = @OS_LDFLAGS@
jack_midisine_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_latent_client_SOURCES = latent_client.c
jack_latent_client_LDFLAGS = @OS_LDFLAGS@
jack_latent_client_LDADD = $(top_builddir)/libjackpp/libjackpp.la

jack_server_control_SOURCES = server_control.c
jack_server_control_LDFLAGS = @OS_LDFLAGS@ $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
jack_server_control_LDADD = $(top_builddir)/jackdpp/libjackserverpp.la $(BOOST_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)

jack_large_number_ports_SOURCES = jack_large_number_ports.cpp
jack_large_number_ports_LDFLAGS = @OS_LDFLAGS@ $(JACK_LDFLAGS)
jack_large_number_ports_LDADD = $(top_builddir)/libjackpp/libjackpp.la

if HAVE_SNDFILE
jack_rec_SOURCES = capture_client.c
jack_rec_LDFLAGS = @SNDFILE_LIBS@ @OS_LDFLAGS@
jack_rec_LDADD = $(top_builddir)/libjackpp/libjackpp.la
endif

#
# sample in-process client(s)
#
ip_clientdir = $(ADDON_DIR)

ip_client_LTLIBRARIES = inprocess.la intime.la

inprocess_la_LDFLAGS = -module -avoid-version @OS_LDFLAGS@
inprocess_la_SOURCES = inprocess.c

intime_la_LDFLAGS = -module -avoid-version @OS_LDFLAGS@
intime_la_SOURCES = intime.c

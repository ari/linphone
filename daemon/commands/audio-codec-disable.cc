#include "audio-codec-disable.h"
#include "audio-codec-get.h"

using namespace std;

AudioCodecDisableCommand::AudioCodecDisableCommand() :
		DaemonCommand("audio-codec-disable", "audio-codec-disable <payload_type_number|mime_type|ALL>",
				"Disable an audio codec.\n"
				"<mime_type> is of the form mime/rate/channels, eg. speex/16000/1") {
}

void AudioCodecDisableCommand::exec(Daemon *app, const char *args) {
	istringstream ist(args);

	if (ist.peek() == EOF) {
		app->sendResponse(Response("Missing parameter.", Response::Error));
	} else {
		string mime_type;
		int ptnum = -1;
		ist >> mime_type;
		PayloadTypeParser parser(app->getCore(), mime_type, true);
		if (!parser.successful()) {
			app->sendResponse(Response("Incorrect mime type format.", Response::Error));
			return;
		}
		if (!parser.all()) ptnum = parser.payloadTypeNumber();

		int index = 0;
		for (const MSList *node = linphone_core_get_audio_codecs(app->getCore()); node != NULL; node = ms_list_next(node)) {
			PayloadType *payload = reinterpret_cast<PayloadType*>(node->data);
			if (parser.all()) {
				linphone_core_enable_payload_type(app->getCore(), payload, false);
			} else {
				if (ptnum == linphone_core_get_payload_type_number(app->getCore(), payload)) {
					linphone_core_enable_payload_type(app->getCore(), payload, false);
					app->sendResponse(PayloadTypeResponse(app->getCore(), payload, index));
					return;
				}
			}
			++index;
		}
		if (parser.all()) {
			AudioCodecGetCommand getCommand;
			getCommand.exec(app, "");
		} else {
			app->sendResponse(Response("Audio codec not found.", Response::Error));
		}
	}
}

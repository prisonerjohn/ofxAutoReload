#pragma once

#include "ofEvents.h"
#include "ofShader.h"

namespace ofxAutoReload
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		void setup(const ofShaderSettings & settings, bool autoUpdate = true);
		void clear();

		void update();
		void reload();

		void setAutoUpdate(bool autoUpdate);

	public:
		ofShader program;
		ofShaderSettings settings;
		std::filesystem::file_time_type loadTime;

		ofEvent<void> reloadEvent;

	private:
		void _update(ofEventArgs & args);

	private:
		bool autoUpdate;
	};
}
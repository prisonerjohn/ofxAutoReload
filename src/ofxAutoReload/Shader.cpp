#include "Shader.h"

#include "ofLog.h"

namespace ofxAutoReload
{
	Shader::Shader()
		: autoUpdate(false)
	{}

	Shader::~Shader()
	{
		this->clear();
	}

	void Shader::setup(const ofShaderSettings & settings, bool autoUpdate)
	{
		this->settings = settings;
		this->loadTime = std::filesystem::file_time_type();
		this->setAutoUpdate(autoUpdate);
		this->update();
	}
	
	void Shader::clear()
	{
		this->setAutoUpdate(false);
		this->loadTime = std::filesystem::file_time_type();
		this->settings = ofShaderSettings();
		this->program.unload();
	}

	void Shader::_update(ofEventArgs & args)
	{
		this->update();
	}

	void Shader::update()
	{
		if (this->settings.shaderFiles.empty())
		{
			ofLogError(__FUNCTION__) << "No shader files set in settings!";
			return;
		}
		auto writeTime = std::filesystem::file_time_type();
		std::filesystem::path filePath;
		for (auto it : this->settings.shaderFiles)
		{
			auto fileTime = std::filesystem::last_write_time(ofToDataPath(it.second));
			if (writeTime < fileTime)
			{
				writeTime = fileTime;
				filePath = it.second;
			}
		}
		if (writeTime > this->loadTime)
		{
			ofLogNotice(__FUNCTION__) << "File " << filePath << " updated on disk, reloading!";
			this->reload();
			this->loadTime = writeTime;
		}
	}

	void Shader::reload()
	{
		this->program.setup(this->settings);
		this->reloadEvent.notify();
	}

	void Shader::setAutoUpdate(bool autoUpdate)
	{
		if (this->autoUpdate == autoUpdate) return;
		this->autoUpdate = autoUpdate;

		if (this->autoUpdate)
		{
			ofAddListener(ofEvents().update, this, &Shader::_update);
		}
		else
		{
			ofRemoveListener(ofEvents().update, this, &Shader::_update);
		}
	}
}

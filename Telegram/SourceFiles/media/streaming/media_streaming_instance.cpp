/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "media/streaming/media_streaming_instance.h"

#include "media/streaming/media_streaming_document.h"
#include "data/data_file_origin.h"
#include "data/data_document.h"
#include "data/data_session.h"

namespace Media {
namespace Streaming {

Instance::Instance(
	std::shared_ptr<Document> shared,
	Fn<void()> waitingCallback)
: _shared(std::move(shared))
, _waitingCallback(std::move(waitingCallback)) {
	if (_shared) {
		_shared->registerInstance(this);
	}
}

Instance::Instance(
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	Fn<void()> waitingCallback)
: Instance(
	document->owner().documentStreamer(document, origin),
	std::move(waitingCallback)) {
}

Instance::~Instance() {
	if (_shared) {
		_shared->unregisterInstance(this);
	}
}

const Player &Instance::player() const {
	Expects(_shared != nullptr);

	return _shared->player();
}

const Information &Instance::info() const {
	Expects(_shared != nullptr);

	return _shared->info();
}

void Instance::play(const PlaybackOptions &options) {
	Expects(_shared != nullptr);

	_shared->play(options);
}

void Instance::pause() {
	Expects(_shared != nullptr);

	_shared->player().pause();
}

void Instance::resume() {
	Expects(_shared != nullptr);

	_shared->player().resume();
}

void Instance::stop() {
	Expects(_shared != nullptr);

	_shared->player().stop();
}

void Instance::saveFrameToCover() {
	Expects(_shared != nullptr);

	_shared->saveFrameToCover();
}

bool Instance::active() const {
	Expects(_shared != nullptr);

	return _shared->player().active();
}

bool Instance::ready() const {
	Expects(_shared != nullptr);

	return _shared->player().ready();
}

bool Instance::paused() const {
	Expects(_shared != nullptr);

	return _shared->player().paused();
}

float64 Instance::speed() const {
	Expects(_shared != nullptr);

	return _shared->player().speed();
}

void Instance::setSpeed(float64 speed) {
	Expects(_shared != nullptr);

	_shared->player().setSpeed(speed);
}

bool Instance::waitingShown() const {
	Expects(_shared != nullptr);

	return _shared->waitingShown();
}

float64 Instance::waitingOpacity() const {
	Expects(_shared != nullptr);

	return _shared->waitingOpacity();
}

Ui::RadialState Instance::waitingState() const {
	Expects(_shared != nullptr);

	return _shared->waitingState();
}

void Instance::callWaitingCallback() {
	if (_waitingCallback) {
		_waitingCallback();
	}
}

QImage Instance::frame(const FrameRequest &request) const {
	return player().frame(request, this);
}

bool Instance::markFrameShown() {
	return _shared->player().markFrameShown();
}

rpl::lifetime &Instance::lifetime() {
	return _lifetime;
}

} // namespace Streaming
} // namespace Media

/*
 *  Tweenzor.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 10/31/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tweenzor.h"

Tweenzor* Tweenzor::__instance = 0;

//--------------------------------------------------------------
Tweenzor* Tweenzor::Instance() {
	if (__instance == 0) {
		__instance = new Tweenzor();
		__instance->_eventIndex = 0;
		__instance->_currMillis = 0;
		removeAllTweens();
	}
	return __instance;
}

//--------------------------------------------------------------
void Tweenzor::init() {
	Instance();
}

//--------------------------------------------------------------
void Tweenzor::update(int a_millis) {
	
	__instance->_currMillis = a_millis;
	
	//cout << "Tweenzor :: update : size() = " << __instance->_tweens.size() << endl;
	
	int _twCount = 0;
	vector<shared_ptr<Tween> >::iterator iter;
	
	vector<int> eventsToFire;
	vector<float*> eventArgs;
	
	
	for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ ) {
		shared_ptr<Tween> &it = *iter;
		if(it->complete()) {
			int evindex = getEventIDForTween( &(*it) );
			if(evindex > -1) {
				//float args = it->getPropertyValue();
				float* arg = it->getProperty();
				//cout << "Tweenzor :: update : is complete = " << _twCount << endl;
				//__instance->_events[evindex]( arg );
				eventsToFire.push_back( evindex );
				eventArgs.push_back( arg );
			}
		}
		//cout << "Tweenzor :: update : count = " << _twCount << endl;
		_twCount ++;
	}
	
	if(__instance->_tweens.size() > 0) {
		int _totesTweens = __instance->_tweens.size();
		for(int i = _totesTweens-1; i >= 0; i--) {
			if(__instance->_tweens[i]->complete()) {
				//removeCompleteListener( &__instance->_tweens[i] );
				removeTween( __instance->_tweens[i]->getProperty() );
			}
		}
		
	}
	
	for(unsigned int i = 0; i < eventsToFire.size(); i++) {
		//cout << "Tweenzor :: update : eventsToFire["<<eventsToFire[i] << "] arg = " << *eventArgs[i] << endl;
		//__instance->_events[eventsToFire[i]]( eventArgs[i] );
		int evindex = getEventIndexForID( eventsToFire[i] );
		__instance->_events[evindex]( eventArgs[i] );
		removeCompleteListener( eventsToFire[i] );
	}
	
	_twCount = 0;
	for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ ) {
		shared_ptr<Tween> &it = *iter;
		it->update( __instance->_currMillis );
		//if(it->complete()) {
			//cout << "Tweenzor :: update : complete ["<<_twCount<<"]" << endl;
		//}
		_twCount++;
	}
	
}

// add a tween that uses frames as time //
//--------------------------------------------------------------
Tween* Tweenzor::add(float* a_property, float a_begin, float a_end, int a_delay, int a_duration, int a_easeType, float a_p, float a_a) {
	removeCompleteListener( a_property );
	removeTween( a_property );
  
	auto tweenzlebob = make_shared<Tween>(a_property, __instance->_currMillis, a_begin, a_end, a_delay, a_duration, a_easeType, a_p, a_a);
	__instance->_tweens.push_back(tweenzlebob);
  
  return tweenzlebob.get();
}

// add a tween in seconds, pass in float for delay and duration //
//--------------------------------------------------------------
Tween* Tweenzor::add(float* a_property, float a_begin, float a_end, float a_delay, float a_duration, int a_easeType, float a_p, float a_a) {
	removeCompleteListener( a_property );
	removeTween( a_property );
	shared_ptr<Tween> tweenzlebob = shared_ptr<Tween>(new Tween( a_property, __instance->_currMillis, a_begin, a_end, a_delay, a_duration, a_easeType, a_p, a_a ));
	__instance->_tweens.push_back( tweenzlebob );
  
  return tweenzlebob.get();
}

//--------------------------------------------------------------
Tween* Tweenzor::add(vector <float *> a_properties, float a_begin, float a_end, float a_delay, float a_duration, int a_easeType, float a_p, float a_a) {
  Tween* t = nullptr;
	for (unsigned int i = 0; i < a_properties.size(); i++) {
    if (i == 0) t = add(a_properties[i], a_begin, a_end, a_delay, a_duration, a_easeType, a_p, a_a);
		else add(a_properties[i], a_begin, a_end, a_delay, a_duration, a_easeType, a_p, a_a);
	}
  
  return t;
}


//------{ addons by @tiagosr }------
Tween* Tweenzor::add(ofFloatColor *a_color, const ofFloatColor &c_begin, const ofFloatColor &c_end, float a_delay, float a_duration, int a_easetype, float a_p, float a_a) {
    Tween* t = add(&a_color->r, c_begin.r, c_end.r, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_color->g, c_begin.g, c_end.g, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_color->b, c_begin.b, c_end.b, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_color->a, c_begin.a, c_end.a, a_delay, a_duration, a_easetype, a_p, a_a);
  
  return t;
}

Tween* Tweenzor::add(ofDefaultVec2 *a_vec, const ofDefaultVec2 &c_begin, const ofDefaultVec2 &c_end, float a_delay, float a_duration, int a_easetype, float a_p, float a_a) {
    Tween* t = add(&a_vec->x, c_begin.x, c_end.x, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_vec->y, c_begin.y, c_end.y, a_delay, a_duration, a_easetype, a_p, a_a);
  
  return t;
}
Tween* Tweenzor::add(ofDefaultVec3 *a_vec, const ofDefaultVec3 &c_begin, const ofDefaultVec3 &c_end, float a_delay, float a_duration, int a_easetype, float a_p, float a_a) {
    Tween* t = add(&a_vec->x, c_begin.x, c_end.x, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_vec->y, c_begin.y, c_end.y, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_vec->z, c_begin.z, c_end.z, a_delay, a_duration, a_easetype, a_p, a_a);
  
  return t;
}

Tween* Tweenzor::add(ofDefaultVec4 *a_vec, const ofDefaultVec4 &c_begin, const ofDefaultVec4 &c_end, float a_delay, float a_duration, int a_easetype, float a_p, float a_a) {
    Tween* t = add(&a_vec->x, c_begin.x, c_end.x, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_vec->y, c_begin.y, c_end.y, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_vec->z, c_begin.z, c_end.z, a_delay, a_duration, a_easetype, a_p, a_a);
    add(&a_vec->w, c_begin.w, c_end.w, a_delay, a_duration, a_easetype, a_p, a_a);
  
  return t;
}

Tween* Tweenzor::add(ofRectangle* a_rect, const ofRectangle& r_begin, const ofRectangle& r_end, float a_delay, float a_duration, int a_easeType, float a_p, float a_a) {
    Tween* t = add(&a_rect->position.x, r_begin.position.x, r_end.position.x, a_delay, a_duration, a_easeType, a_p, a_a);
    add(&a_rect->position.y, r_begin.position.y, r_end.position.y, a_delay, a_duration, a_easeType, a_p, a_a);
    add(&a_rect->position.z, r_begin.position.z, r_end.position.z, a_delay, a_duration, a_easeType, a_p, a_a);
    add(&a_rect->width, r_begin.width, r_end.width, a_delay, a_duration, a_easeType, a_p, a_a);
    add(&a_rect->height, r_begin.height, r_end.height, a_delay, a_duration, a_easeType, a_p, a_a);
  
  return t;
}



//--------------------------------------------------------------
Tween* Tweenzor::add( TweenParams& a_params ) {
	if(a_params.bUseSeconds) {
		return add(a_params.property, a_params.begin, a_params.end, a_params.delay, a_params.duration, a_params.easeType, a_params.p, a_params.a);
	} else {
		return add(a_params.property, a_params.begin, a_params.end, (int)a_params.delay, (int)a_params.duration, a_params.easeType, a_params.p, a_params.a);
	}
}

//--------------------------------------------------------------
void Tweenzor::add( vector<TweenParams>& a_params ) {
	for(unsigned int i = 0; i < a_params.size(); i++) {
		add( a_params[i] );
	}
}

//--------------------------------------------------------------
void Tweenzor::removeTween( float* a_property ) {
	if (__instance->_tweens.size() > 0) {
		int i = 0;
		vector<shared_ptr<Tween> >::iterator iter;
		for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ )
		{
			shared_ptr<Tween> it = *iter;
			if ( it->getProperty() == a_property ) {
				//cout << "Tweenzor :: removeTween : property = " <<  it->getProperty() << " = " << a_property << endl;
				it->remove();
				__instance->_tweens.erase( iter );
				break;
			}
			i++;
		}
		
	}
}

//--------------------------------------------------------------
Tween* Tweenzor::getTween( float* a_property ) {
	vector<shared_ptr<Tween> >::iterator iter;
	for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ ) {
		shared_ptr<Tween> &it = *iter;
		if(it->getProperty() == a_property) {
			return ((Tween*) &(*it));
		}
	}
	return NULL;
}

Tween* Tweenzor::getTween(ofFloatColor *color) {
    return getTween(&color->r);
}

Tween* Tweenzor::getTween(ofDefaultVec2 *vec) {
    return getTween(&vec->x);
}

Tween* Tweenzor::getTween(ofDefaultVec3 *vec) {
    return getTween(&vec->x);
}
Tween* Tweenzor::getTween(ofDefaultVec4 *vec) {
    return getTween(&vec->x);
}

Tween* Tweenzor::getTween(ofRectangle *rect) {
    return getTween(&rect->position.x);
}


// protected //
//--------------------------------------------------------------
int Tweenzor::getEventIndexForTween( Tween* a_tween ) {
	if(a_tween == NULL) return -1;
	unsigned int evid = a_tween->eventID;
	for(unsigned int i = 0; i < __instance->_events.size(); i++) {
		if( (__instance->_events[i].getID()) == evid ) {
			return i;
			break;
		}
	}
	return -1;
}

//--------------------------------------------------------------
int Tweenzor::getEventIDForTween( Tween* a_tween ) {
	if(a_tween == NULL) return -1;
	unsigned int evid = a_tween->eventID;
	for(unsigned int i = 0; i < __instance->_events.size(); i++) {
		if( (__instance->_events[i].getID()) == evid ) {
			return __instance->_events[i].getID();
			break;
		}
	}
	return -1;
}

//--------------------------------------------------------------
int Tweenzor::getEventIndexForID( int a_eventID ) {
	for(unsigned int i = 0; i < __instance->_events.size(); i++) {
		if( (__instance->_events[i].getID()) == a_eventID ) {
			return i;
		}
	}
	return -1;
}

//--------------------------------------------------------------
void Tweenzor::removeCompleteListener( Tween* a_tween ) {
	if(a_tween == NULL) return;
	unsigned int evid = a_tween->eventID;
	for(unsigned int i = 0; i < __instance->_events.size(); i++) {
		if( (__instance->_events[i].getID()) == evid ) {
			if(__instance->_events[i] == true) {
				__instance->_events[i].destroy();
				__instance->_events[i] = NULL;
				__instance->_events.erase( __instance->_events.begin() + i);
			}
			break;
		}
	}
}

//--------------------------------------------------------------
void Tweenzor::removeCompleteListener( float* a_property ) {
	removeCompleteListener( getTween(a_property) );
}

//--------------------------------------------------------------
void Tweenzor::removeCompleteListener( int a_eventID ) {
	for(unsigned int i = 0; i < __instance->_events.size(); i++) {
		if( (__instance->_events[i].getID()) == a_eventID ) {
			__instance->_events[i].destroy();
			__instance->_events[i] = NULL;
			__instance->_events.erase( __instance->_events.begin() + i);
			break;
		}
	}
}

//--------------------------------------------------------------
void Tweenzor::removeAllListeners() {
	__instance->_events.clear();
}

//--------------------------------------------------------------
void Tweenzor::removeAllTweens() {
	for(unsigned int i = 0; i < __instance->_events.size(); i++) {
		__instance->_events[i].destroy();
		__instance->_events[i] = NULL;
	}
	
	__instance->_events.clear();
	__instance->_tweens.clear();
	
	cout << "Tweenzor :: removeAllTweens : " << __instance->_tweens.empty() << endl;
}

//--------------------------------------------------------------
void Tweenzor::destroy() {
	removeAllTweens();
	
	if(__instance != 0) {
		delete __instance;
		__instance = 0;
		
		cout << "Tweenzor :: destroy : " << endl;
	}
	
}

//--------------------------------------------------------------
void Tweenzor::pauseAllTweens() {
	if (__instance->_tweens.size() > 0) {
		vector<shared_ptr<Tween> >::iterator iter;
		for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ ) {
			(*iter)->pause();
		}
	}
}

//--------------------------------------------------------------
void Tweenzor::resetAllTweens() {
	if (__instance->_tweens.size() > 0) {
		vector<shared_ptr<Tween> >::iterator iter;
		for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ ) {
			(*iter)->reset(__instance->_currMillis);
		}
	}
}

//--------------------------------------------------------------
void Tweenzor::resumeAllTweens() {
	if (__instance->_tweens.size() > 0) {
		vector<shared_ptr<Tween> >::iterator iter;
		for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ )
		{
			(*iter)->resume(__instance->_currMillis);
		}
	}
}

// toggle pause / resume all tweens //
//--------------------------------------------------------------
void Tweenzor::toggleAllTweens() {
	if (__instance->_tweens.size() > 0) {
		vector<shared_ptr<Tween> >::iterator iter;
		for ( iter=__instance->_tweens.begin(); iter < __instance->_tweens.end(); iter++ ) {
			(*iter)->toggle(__instance->_currMillis);
		}
	}
}

//--------------------------------------------------------------
Tween* Tweenzor::getRecentTween() {
	if (__instance->_tweens.size() > 0) {
		return __instance->_tweens[__instance->_tweens.size() - 1].get();
	}
  return 0;
}

//--------------------------------------------------------------
int Tweenzor::getSize() {
	return __instance->_tweens.size();
}








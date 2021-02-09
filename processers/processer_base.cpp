#include "processers/processer_base.h"
#include "net/net_common.h"
#include "world/world.h"
#include <iostream>
#include <thread>
#include <chrono>

Processer_Base::Processer_Base(std::vector<Session*>* _vec_sessions) : vec_sessions(_vec_sessions) {}
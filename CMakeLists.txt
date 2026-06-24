cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
# -> CMake 최소 버전 3.10 이상 필요

project(Tutorial)
# -> 프로젝트 이름을 Tutorial로 설정

find_package(Geant4 REQUIRED ui_all vis_all)
# -> Geant4 패키지를 필수로 찾음

include(${Geant4_USE_FILE})
# -> Geant4가 제공하는 CMake 설정을 가져옴
# 컴파일 옵션, include 경로, 라이브러리 경로 등 "Geant4 쓸 때 필요한 세팅" 자동 셋업

include_directories(${PROJECT_SOURCE_DIR}/include)
# -> ./include 폴더 안에 헤더들을 #include할 수 있게 경로 추가

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
# -> ./src 폴더 안의 모든 .cc 파일을 sources 변수에 넣음. 나중에 실행파일 만들 때 다 같이 컴파일 하게 하려고. 

file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)

add_executable(sim sim.cc ${sources})
 # -> sim 이라는 실행 파일을 만든다. sin.cc: 메인 함수 있는 파일, ${sources} : src 안에 있는 나머지 구현 파일들
target_link_libraries(sim ${Geant4_LIBRARIES})
# -> sim을 Geant4 라이브러리들과 링크

add_custom_target(Tutorial DEPENDS sim)
# -> Tutorial 이라는 가짜(target)을 만들고, 이 Target은 sim이 ajswj qlfemehldj dlTdjdigka. 
# -> 나중에 make Tutorial 하면 sim이 빌드되는 구조.
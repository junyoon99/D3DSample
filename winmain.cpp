#include "D3DFramework.h"

// Direct3D
// 1. 디바이스 생성 (디바이스 컨텍스트 포함)
// 2. 스왑체인 생성
// 3. 렌더타겟으로 후면 버퍼 지정
// 4. 깊이버퍼 + 스텐실버퍼 생성
// 5. 파이프라인 설정
// 6. 뷰포트 지정 View port

// 7. 그리기
	// 렌더타겟
	// 깊이스텐실 버퍼

// 모드 스위치 룰
// 0. 해상도는 옵션으로 변경 - 변수
// 0. 윈도우 배경색 칠하기 과정을 생략
// 1. 스왑체인의 속성 변경
// 2. 게임 종료시에 반드시 전체화면 모드 해제 후 종료

// 가변 해상도
// 스왑체인
// 렌더타겟
// 깊이스텐실
// 뷰포트

// 윈도우 창 크기 변경 메시지
// WM_SIZE, WM_ENTERSIZEMOVE, WM_EXITSIZEMOVE
// 크기 조절 제한, 메뉴 없음 경고음 제거


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) 
{
	D3DFramework mySample;
	mySample.Initialize(hInstance);

	mySample.GameLoop();

	mySample.Destroy();

	return 0;
}

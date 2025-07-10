# 6 DOF 드론 구현 프로젝트

이 프로젝트는 언리얼 엔진에서 6DOF를 갖는 드론을 구현하는 데에 초점을 맞추어 제작되었습니다. 
커스텀 `UDroneMovementComponent`를 사용하여, 플레이어는 드론과 유사한 Pawn을 6DOF로 조종할 수 있습니다.

---
## 주요 기능 

- **드론 전용 Pawn 클래스 (`ADronePlayer`)**
  - 6자유도 조종 (이동, 회전, 롤링)
  - Spring Arm을 통한 3인칭 카메라 지원
  - 충돌 감지를 위한 Sphere Collision 사용
 
  
- **사용자 정의 이동 컴포넌트 (`UDroneMovementComponent`)**
  - 중력 인식 이동 및 공중 제어 계수
  - 추력 기반 이동 및 감속 처리
  - Sweep 기반 지면 감지
  - 커스텀 충돌 반응 및 슬라이딩 처리
 
---

## 드론의 물리 기반 이동 시스템

`UDroneMovementComponent`는 물리 기반 이동 시스템으로, 아래의 요소들을 계산하여 Pawn의 이동과 회전 등을 처리합니다.

- 입력 기반 추력
- 중력 및 항력 계산
- 지면 접촉 여부 반영
- 속도 감속 보간
- 회전 및 각속도 적용

## 입력 처리 순서 

1. **입력 처리**: 지면 여부에 따라 입력 보정  
2. **추력 계산**: 방향별 추력 + 중력 + 항력 적용  
3. **속도 갱신**: 가속도를 적용한 속도 계산  
4. **이동 적용**: `SafeMoveUpdatedComponent()`로 위치 갱신  
5. **충돌 처리**: 슬라이딩 벡터 계산 및 반영  
6. **회전 처리**: 각속도에 따른 회전 적용  
7. **지면 감지**: Sphere Sweep을 통한 접지 상태 확인  
8. **입력 초기화**: 다음 프레임을 위한 입력값 초기화

---

## 물리 파라미터 설명 
| 변수명                 | 타입       | 카테고리         | 설명 |
|------------------------|------------|------------------|------|
| `Mass`                 | float      | DronePhysics      | 드론의 질량 |
| `Gravity`              | float      | DronePhysics      | 중력 가속도 (엔진에서 받아옴) |
| `MaxThrustForce`       | float      | DronePhysics      | 수평 방향 최대 추력 |
| `MaxVerticalThrust`    | float      | DronePhysics      | 수직 방향 최대 추력 |
| `DragCoefficient`      | float      | DronePhysics      | 속도에 따른 항력 계수 |
| `DampingSpeed`         | float      | DronePhysics      | 입력이 없을 때 속도 감속 보간 계수 |
| `MaxSpeed`             | float      | DronePhysics      | 최대 속도 제한 |
| `AirControlFactor`     | float      | DronePhysics      | 공중 제어 민감도 (지면에 비해 감소) |
| `GroundCheckDistance`  | float      | Ground Check      | 지면 감지 거리 (Sweep 수행 거리) |
| `bIsOnGround`          | bool       | 내부 사용         | 드론이 지면에 있는지 여부 |

---

자세한 드론 물리 관련 설명은 아래의 Notion 링크를 확인해주시길 바랍니다. 
https://www.notion.so/22bdba07f02e8061953fd1675630b65d?source=copy_link

---


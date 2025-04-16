# Algorhythm(알고리듬)
![Frame 13](https://github.com/user-attachments/assets/6c181945-5bea-4784-8f3b-75f0755b8731)<br/>

알고리즘 일정 관리 프로그램 <b>Algorhythm</b>(Algorithm + Rhythm)과 함께 일상의 리듬을 만들어보세요.

## 프로젝트 기획 배경 및 소개
<b>Algorhythm(알고리듬)</b>은 알고리즘 일정 관리 프로그램입니다.

매일 알고리즘을 푸는 VEDA 구성원들을 보며 알고리즘 일정을 관리할 수 있는 프로그램을 기획하게 되었습니다.
<br/><br/>

알고리듬은 크게 USER탭과 TODO탭으로 나뉘어져 있습니다.

USER 탭에서는 백준 기반의 [solved.ac](http://solved.ac) API를 활용하여 사용자의 문제 풀이 현황 및 추천 문제를 관리합니다.

TODO 탭은 오늘의 할 일을 정리하는 투두 리스트와 캘린더로 이루어져 있습니다.


## 프로젝트 목표
Algorhythm은 사용자의 알고리즘 문제 풀이 학습 및 일정 관리를 돕는 것이 목표입니다.

<b>USER:</b>
- 사용자 티어 관리: [solved.ac](http://solved.ac) 연동을 통하여 사용자의 현재 티어와 문제 풀이 현황을 시각적으로 제공
- 학습 통계 : 티어별 해결한 문제 수를 차트로 시각화하여 학습 진행 상황 파악
- 문제 추천 기능 : 사용자가 이전에 푼 문제를 바탕으로 문제 추천
- 문제 검색 기능 : 백준 문제 번호로 검색하여 정보 확인
<br/><br/>

<b>TODO:</b>
- 할 일 관리 : 날짜 별로 할 일을 등록하고 체크할 수 있는 기능
- 태그 기능 : 태그를 활용하여 해당 TODO가 작성된 날짜 검색 기능


## 이용 방법
![로그임](https://github.com/user-attachments/assets/8c7eaff6-c1ed-4693-bc92-ca87c2692966)<br/>

Algorhythm을 실행한 후 로그인버튼을 클릭합니다.

![image](https://github.com/user-attachments/assets/46f9484d-131e-4c70-a72f-293da7d0d1a6)<br/>

백준에 연동된 solved.ac 아이디를 입력합니다.<br/>
아이디가 없을 경우 Algorhythm 개발자 아이디인 <b>nlnf44</b>를 입력하여 테스트해 볼 수 있습니다.

## 기능 및 UI 소개

### USER - 로그아웃 상태
![1](https://github.com/user-attachments/assets/09b0ab36-17b5-4577-a64e-ba4e736e7540)<br/>
로그아웃 상태의 USER 탭입니다.<br/>
로그인 상태가 아닐 경우 문제 검색, 그래프, 랜덤 문제 출력 기능을 이용할 수 없습니다.
<br/><br/>

### USER - 로그인
![2](https://github.com/user-attachments/assets/3d265150-59f6-4f94-a600-d2157cc9ce45)<br/>
로그인 버튼을 클릭하면 dialog를 통하여 solved.ac에 연동된 백준 아이디 입력 창이 뜹니다.<br/> 
입력창에 사용자의 아이디를 입력하고 로그인 버튼을 클릭합니다. 
<br/><br/>

### USER - 존재하지 않는 아이디 입력했을 경우
![3](https://github.com/user-attachments/assets/a17fb13a-1e51-4c13-8d44-3d4bf4ca80b1)<br/> 
만약 존재하지 않거나 비공개 아이디를 입력하면 "아이디를 다시 확인해주세요"라는 문구를 출력합니다.
<br/><br/>

### USER - 로그인 완료
![4](https://github.com/user-attachments/assets/1330f6f3-b096-4511-ba24-d01acbad462d)<br/>
로그인이 완료되면
<br/>
- user greeting: 사용자의 현재의 티어
- user chart: 티어 별 푼 문제 수 그래프
- user search: 문제 검색 기능
- user recommend: 사용자가 푼 문제 중 5문제 랜덤 출력

기능이 활성화됩니다.
<br/><br/>

### USER - user greeting
![5](https://github.com/user-attachments/assets/e5dafe6e-69f2-4cf0-a41a-4e98452c3730)<br/>
로그인 시 계정이 연동되어 현재의 사용자의 티어와 티어 이미지가 출력됩니다.<br/>
버튼을 눌러 로그인/로그아웃을 할 수 있습니다.
<br/><br/>

### USER - user chart
![6](https://github.com/user-attachments/assets/7ad04d30-b8ea-4314-b88f-33b7c7efd661)<br/>
사용자가 푼 문제를 티어별로 나눠 그래프로 출력합니다.<br/>
각 그래프에 마우스를 가져다 대면 푼 문제 수가 tooltip으로 표시됩니다. 
<br/><br/>

### USER - user search
![7](https://github.com/user-attachments/assets/3847b98c-8bb9-4b35-abb9-dbaebde24e02)<br/>
백준 문제를 문제번호로 검색할 수 있습니다.<br/>
문제 출력 부분에 마우스를 가져다 대면 "더블클릭 시 해당 문제의 페이지로 이동합니다"라고 tooltip이 표시됩니다.<br/>
또한 더블 클릭 시 문제 페이지로 연결됩니다.
<br/><br/>

### USER - user recommend
![8](https://github.com/user-attachments/assets/a761316c-c3de-498f-8330-ac0c8ef73408)<br/>
사용자가 푼 문제 중 랜덤으로 5문제가 출력되고 각각 어느 티어의 문제인지 보여줍니다.<br/>
문제 부분에 마우스를 가져다 대면 "더블클릭 시 해당 문제의 페이지로 이동합니다"라고 tooltip이 표시됩니다.
<br/><br/>
![9](https://github.com/user-attachments/assets/ba9267eb-c1cb-493d-9bb0-ab821c947669)
문제 더블 클릭시 백준 문제 페이지로 이동합니다.

<br/><br/>

### TODO
![10](https://github.com/user-attachments/assets/d9fa1bd8-4995-4e5f-b850-22a68e8e5fa6)<br/>
TODO탭은 캘린더, 태그 검색, 할 일 입력/출력/삭제/수정/저장 기능이 있습니다.
<br/><br/>

### TODO - 캘린더 날짜 이동 및 할 일 입력/수정
![11](https://github.com/user-attachments/assets/d750e578-da56-4866-af45-c9e96f41e983)<br/>
작성된 TODO가 없는 캘린더 날짜를 더블클릭하면 dialog를 통하여 "작성된 TODO가 없습니다."라고 사용자에게 알려줍니다.<br/>

![12](https://github.com/user-attachments/assets/5c6b1f51-3bcf-4e80-89aa-d1a6c2a364ca)<br/>
작성된 TODO가 있을 경우 해당 날짜의 TODO를 불러옵니다.<br/>
오른쪽 상단에 태그 및 할 일을 작성한 후 "+" 버튼 클릭 시 할 일 목록에 추가됩니다.<br/>
태그를 작성하지 않고 할 일 을 추가할 경우 태그를 작성하라는 알림창을 띄웁니다.<br/>
checkbox를 클릭하면 할 일을 다 한 것으로 판단하여 ~~백준 버블 정렬 문제 풀기~~ 와 같이 표시 됩니다.<br/>
휴지통 아이콘 클릭 시 해당 줄의 할 일이 삭제됩니다.<br/>
저장하기 버튼 클릭 시 모두 저장됩니다.<br/>

![13](https://github.com/user-attachments/assets/6f1fe33f-81fe-499f-a8c3-c9f0917b1b4b)<br/>
할 일 상태 변경, 삭제, 태그 변경, 할 일 추가 등의 변화가 있음에도 불구하고 저장하기를 누르지 않고 캘린더 날짜를 이동하면 dialog를 통하여 "저장하지 않은 작업이 있습니다. 그래도 이동하시겠습니까?"라는 알림창을 띄웁니다.<br/>
yes 클릭 시 저장하지 않고 날짜를 이동합니다.<br/>
No를 누르면 이동하지 않고 원래의 화면을 유지합니다.<br/>

<br/><br/>

### TODO - 할 일 삭제
![14](https://github.com/user-attachments/assets/4c3fe631-9d31-4b88-95c7-be09fc21ef42)<br/>
저장하기 버튼 옆의 삭제하기 버튼을 누르면 "정말 삭제하시겠습니까?" 라고 dialog 창을 띄웁니다.<br/>

![15](https://github.com/user-attachments/assets/3eb35f60-5c51-42f4-84b9-4a4a924f570c)<br/>
yes 버튼 클릭 시 해당 할 일을 삭제합니다.
<br/><br/>

### TODO - 태그 검색
![16](https://github.com/user-attachments/assets/c3e6a009-4172-44b6-b05a-0fe3fe39d8f3)<br/>
TODO 작성 시에 입력한 태그로 작성한 TODO를 검색할 수 있습니다.<br/>
검색 후 태그가 있을 시 해당 태그가 있는 모든 년-월-일을 출력합니다.<br/> 
더블 클릭 시 해당 캘린더에 작성된 TODO를 불러옵니다. <br/>

![17](https://github.com/user-attachments/assets/8a501e2f-a4fc-427d-af35-27f0afad53dc)<br/>
검색한 태그를 가진 TODO가 존재하지 않으면 "해당 태그를 가진 날짜가 없습니다"라는 메세지를 출력합니다.


## 프로젝트 구조
```
Algorhythm/
├── Forms/
│   └── algorhythm.ui
├── Header Files/
│   ├── algorhythm.h
│   ├── networkmanager.h
│   ├── todolist.h
│   ├── userchart.h
│   ├── usergreeting.h
│   ├── userrecommend.h
│   ├── usersearch.h
│   └── usertiercard.h
├── Resources/
│   └── Images/
└── Source Files/
    ├── algorhythm.cpp
    ├── main.cpp
    ├── networkmanager.cpp
    ├── todolist.cpp
    ├── userchart.cpp
    ├── usergreeting.cpp
    ├── userrecommend.cpp
    ├── usersearch.cpp
    └── usertiercard.cpp
```


## UML
<img width="12400" alt="UML Diagrams qt2" src="https://github.com/user-attachments/assets/29229e0d-868a-4689-94da-be347fa27342" />

## 참고 자료
[solved.ac API](https://solvedac.github.io/unofficial-documentation/#/) <br/>
[DOC Qt](https://doc.qt.io/qt-6/qtcharts-overview.html) 

## 팀 소개
VEDA 4조🍀<br/>
[김민정](https://github.com/MIN60)
[남윤서](https://github.com/Yunseo3)












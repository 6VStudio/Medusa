1. Create window and opengl view before start game

Todo: 
1. 2D��Ⱦ�ɲ���������Ż�,�����ڵ���ϵ�Ż�,����4�����Ż�
2.��RenderState����Pool,�Լ���Ƶ���Ĵ�����ɾ��

Note:
XcodeҲ����#pragma once


Application:
Window:
[Views]


��Ϸͳ����Ϣ
1��FPS
fps��ʵ���� frames per second,Ҳ����ÿһ����Ϸִ�е�֡��,�����ֵԽС,˵����ϷԽ����
 
2��Draw calls
batching֮����Ⱦmesh������,�͵�ǰ��Ⱦ��������Ĳ����������йء�
 
3��Saved by batching 
��Ⱦ������������,�������潫�������Ļ��ƽ��кϲ��Ӷ�����CPU�Ŀ���;
�ܶ�GUI�����һ���ô����Ǻϲ�����������Ⱦ,�Ӷ�����DrawCalls ,��֤��Ϸ֡����
 
4��Tris ��ǰ���Ƶ���������
 
5��Verts ��ǰ���ƵĶ�����
 
6��Used Textures ��ǰ֡������Ⱦ��ͼƬռ���ڴ��С
 
7��Render Textures ��Ⱦ��ͼƬռ���ڴ��С,Ҳ���ǵ�Ȼ��Ⱦ������Ĳ����ϵ��������ڴ�ռ��
 
8��VRAM usage �Դ��ʹ�����,VRAM�ܴ�Сȡ��������Կ����Դ�
 
9��VBO Total ��Ⱦ���������ص�ͼ�ο������������,����ע��һ��������ŵ����������Ҫ����Ŀ�����
 
10��Visible Skinned Meshes ��Ƥ�������Ⱦ����
 
11��Animations ���Ŷ���������


TODO:
FreeType�����Ӱ��Ч����δʵ����ȫ

UIϵͳ����
1. fixed position��Ҳ����cocos2d-x 1.x�汾,cocos2d-iphoneĿǰ�汾��ˮƽ
2. relative position��Ҳ����cocos2d-x 2.x��ˮƽ
3. layout, v-box, h-box,Ҳ����Android GUIϵͳ,cocos2d-x 3.0 GUI��ˮƽ
4. web����css, htmlͼ�Ļ��ŵ�ˮƽ��Ŀǰ���ǻ�û������


XAML����
1.֧��ʸ��ͼ,��GPU����Shader����ȾSVG
2.���̼ܹ߳�
3.UIԪ�ص����Կ���Ӧ�ö���
4.����֧��3D��Ⱦ
5.֧��Ƥ��ϵͳ
6.UI�ʹ���ֿ�


WPF����ԭ��:
��Ӧ��ʽ�趨Ԫ�صĳߴ�
��Ӧʹ�þ�������
��������Ӧ�þ�����Ϊ����Ԫ�����ú��ʵĳߴ������ÿռ�
������������Ƕ��

���ֵĽ׶�:
����,������ѯ��������Ԫ�صĳߴ�
����,�ں��ʵ�λ�÷�����Ԫ��

Panel:
StackPanel:��һ��ˮƽ��ֱ�Ķ�ջ�з���Ԫ��,������
WrapPanel:��һϵ�пɻ��е����з���Ԫ��,��ˮƽ��ֱ
DockPanel:���ݰ������������߽����Ԫ��
Grid:����һ�����ɼ����������к����а���Ԫ��
UniformGrid:����һ�����ɼ�����ǿ���������������ͬ�ߴ�������а���Ԫ��
Canvas:ʹ�ù̶������궨λԪ��


C++11����֧��
�Ǿ�̬��Ա��ʼ��
final �ؼ���
�̳й��캯�� using Base::Base
ί�й��캯�� Info(int):Info(){}
��ֵ���� move,�ƶ�����,�ƶ���ֵ
����ת�� forward
��ʼ���б�{}
ģ����� using a=
����Ӧ��auto
decltype
׷�ٷ�������auto sum(T1,T2)->decltype(T1+T2){}
constexpr
�䳤����ģ��
=default �ָ�pod
=delete ɾ��
lambda
emplace_back,emplace,emplace_front
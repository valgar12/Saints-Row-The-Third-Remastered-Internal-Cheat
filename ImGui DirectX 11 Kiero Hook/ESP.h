#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Vector.h"
#include <array>
#include "globals.h"

inline Vec2 ScreenCenter{ 1920 / 2, 1080 / 2 };

namespace ESP
{
	inline float LerpF(float a, float b, float t) { return a + (b - a) * t; }
	inline ImColor LerpColor(const ImColor& a, const ImColor& b, float t) {
		return ImColor(
			(int)LerpF((float)a.Value.x * 255.0f, (float)b.Value.x * 255.0f, t),
			(int)LerpF((float)a.Value.y * 255.0f, (float)b.Value.y * 255.0f, t),
			(int)LerpF((float)a.Value.z * 255.0f, (float)b.Value.z * 255.0f, t),
			(int)LerpF((float)a.Value.w * 255.0f, (float)b.Value.w * 255.0f, t)
		);
	}
	inline float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

	inline void DrawBox(Vec2 A, Vec2 B, ImColor color, bool outline = true, float rounding = 0.f, float t = 1.f)
	{
		if (!outline)
			draw_list->AddRect(A.ToImVec2(), B.ToImVec2(), color, rounding, t);
		else
		{
			draw_list->AddRect(A.ToImVec2(), B.ToImVec2(), color, rounding, t);
			draw_list->AddRect({ A.x + 1, A.y + 1 }, { B.x - 1, B.y - 1 }, Colors::Black, rounding, t);
			draw_list->AddRect({ A.x - 1, A.y - 1 }, { B.x + 1, B.y + 1 }, Colors::Black, rounding, t);
		}
	}
	inline void DrawLine(Vec2 A, Vec2 B, ImColor color, bool outline = true, float t = 1.f)
	{
		if (!outline)
			draw_list->AddLine(A.ToImVec2(), B.ToImVec2(), color, t);
		else
		{
			draw_list->AddLine(A.ToImVec2(), B.ToImVec2(), Colors::Black, t + 1);
			draw_list->AddLine(A.ToImVec2(), B.ToImVec2(), color, t);
		}
	}
	inline void DrawCircle(Vec2 pos, float r, ImColor color, int segments = 12, float t = 1.f)
	{
		draw_list->AddCircle(pos.ToImVec2(), r, color, segments, t);
	}
	inline void DrawCircleFilled(Vec2 pos, float r, ImColor color, int segments = 12)
	{
		draw_list->AddCircleFilled(pos.ToImVec2(), r, color, segments);
	}
	inline void DrawBoxFilled(Vec2 A, Vec2 B, ImColor color, float rounding = 0.f, float t = 1.f)
	{
		draw_list->AddRectFilled(A.ToImVec2(), B.ToImVec2(), color, rounding);
	}
	inline void DrawCorneredBox(Vec2 A, Vec2 B, float w, float h, ImColor color, bool outline = true)
	{
		const float wi = w / 3;
		const float hi = h / 3;

		DrawLine({ A.x - w, A.y }, { A.x - w + wi, A.y }, color);
		DrawLine({ A.x + w, A.y }, { A.x + w - wi, A.y }, color);

		DrawLine({ A.x - w, A.y }, { A.x - w, A.y + hi }, color);
		DrawLine({ A.x + w, A.y }, { A.x + w, A.y + hi }, color);

		DrawLine({ B.x - w, B.y }, { B.x - w + wi, B.y }, color);
		DrawLine({ B.x + w, B.y }, { B.x + w - wi, B.y }, color);

		DrawLine({ B.x - w, B.y }, { B.x - w, B.y - hi }, color);
		DrawLine({ B.x + w, B.y }, { B.x + w, B.y - hi }, color);
	}
	inline void DrawCrosshair(float size, float t, ImColor color)
	{
		DrawLine({ (float)*GameResX / 2 - size, (float)*GameResY / 2 }, { (float)*GameResX / 2 + size, (float)*GameResY / 2 }, color, true, t);
		DrawLine({ (float)*GameResX / 2 , (float)*GameResY / 2 - size }, { (float)*GameResX / 2, (float)*GameResY / 2 + size }, color, true, t);
	}
	inline void DrawText2(Vec2 pos, ImColor color, const char* text)
	{
		draw_list->AddText(ImVec2(pos.x, pos.y), color, text);
	}
	inline void Draw3DBoxFromCenterExtentsRelative(Vec3& objPos, const Vec3& centerRel, const Vec3& extents, ImColor color, float thickness = 1.0f)
	{
		Vec3 center;
		center.x = objPos.x + centerRel.x;
		center.y = objPos.y + centerRel.y;
		center.z = objPos.z + centerRel.z;

		Vec3 mn{ center.x - extents.x, center.y - extents.y, center.z - extents.z };
		Vec3 mx{ center.x + extents.x, center.y + extents.y, center.z + extents.z };

		Vec3 v[8] = {
			{ mn.x, mn.y, mn.z }, { mx.x, mn.y, mn.z }, { mx.x, mx.y, mn.z }, { mn.x, mx.y, mn.z },
			{ mn.x, mn.y, mx.z }, { mx.x, mn.y, mx.z }, { mx.x, mx.y, mx.z }, { mn.x, mx.y, mx.z }
		};

		const int edges[12][2] = {
			{0,1},{1,2},{2,3},{3,0},
			{4,5},{5,6},{6,7},{7,4},
			{0,4},{1,5},{2,6},{3,7}
		};

		Vec2 pts2d[8];
		bool valid[8] = { false };
		for (int i = 0; i < 8; ++i) {
			alignas(16) Vec4 in { v[i].x, v[i].y, v[i].z, 1.0f };
			alignas(16) Vec4 out;
			if (WorldToScreen(reinterpret_cast<__m128*>(&out), reinterpret_cast<__m128*>(&in))) {
				pts2d[i].x = out.x; pts2d[i].y = out.y; valid[i] = true;
			}
			else valid[i] = false;
		}

		for (int e = 0; e < 12; ++e) {
			int a = edges[e][0], b = edges[e][1];
			if (valid[a] && valid[b]) {
				ESP::DrawLine(pts2d[a], pts2d[b], color, true, thickness);
			}
		}

		/*alignas(16) Vec4 inC { center.x, center.y, center.z, 1.0f };
		alignas(16) Vec4 outC;
		if (WorldToScreen(reinterpret_cast<__m128*>(&outC), reinterpret_cast<__m128*>(&inC))) {
			Vec2 c{ outC.x, outC.y };
			float s = 6.0f;
			ESP::DrawLine({ c.x - s, c.y }, { c.x + s, c.y }, color, true, thickness);
			ESP::DrawLine({ c.x, c.y - s }, { c.x, c.y + s }, color, true, thickness);
		}*/
	}

	inline void DrawBoxFromMinMax(const Vec3& mn, const Vec3& mx, ImColor color, float thickness = 1.0f)
	{
		Vec3 v[8] = {
			{ mn.x, mn.y, mn.z }, 
			{ mx.x, mn.y, mn.z }, 
			{ mx.x, mx.y, mn.z }, 
			{ mn.x, mx.y, mn.z }, 
			{ mn.x, mn.y, mx.z }, 
			{ mx.x, mn.y, mx.z }, 
			{ mx.x, mx.y, mx.z }, 
			{ mn.x, mx.y, mx.z }  
		};

		const int edges[12][2] = {
			{0,1},{1,2},{2,3},{3,0}, 
			{4,5},{5,6},{6,7},{7,4},
			{0,4},{1,5},{2,6},{3,7} 
		};

		Vec2 pts2d[8];
		bool valid[8] = { false };
		for (int i = 0; i < 8; ++i) {
			alignas(16) Vec4 in { v[i].x, v[i].y, v[i].z, 1.0f };
			alignas(16) Vec4 out;
			if (WorldToScreen(reinterpret_cast<__m128*>(&out), reinterpret_cast<__m128*>(&in))) {
				pts2d[i].x = out.x;
				pts2d[i].y = out.y;
				valid[i] = true;
			}
			else {
				valid[i] = false;
			}
		}

		for (int e = 0; e < 12; ++e) {
			int a = edges[e][0];
			int b = edges[e][1];
			if (valid[a] && valid[b]) {
				ESP::DrawLine(pts2d[a], pts2d[b], color, true, thickness);
			}
		}

		Vec3 center = { (mn.x + mx.x) * 0.5f, (mn.y + mx.y) * 0.5f, (mn.z + mx.z) * 0.5f };
		alignas(16) Vec4 inC { center.x, center.y, center.z, 1.0f };
		alignas(16) Vec4 outC;
		if (WorldToScreen(reinterpret_cast<__m128*>(&outC), reinterpret_cast<__m128*>(&inC))) {
			Vec2 c{ outC.x, outC.y };
			float s = 6.0f;
			ESP::DrawLine({ c.x - s, c.y }, { c.x + s, c.y }, color, true, thickness);
			ESP::DrawLine({ c.x, c.y - s }, { c.x, c.y + s }, color, true, thickness);
		}
	}

	inline void DrawHealthBarLeft(const Vec2& top, const Vec2& bottom, float maxHP, float curHP, float boxHalfWidth, bool topIsCenterX = true) {
		if (maxHP <= 0.f) return;
		float frac = curHP / maxHP;
		frac = (frac < 0.f) ? 0.f : (frac > 1.f ? 1.f : frac);

		const float rounding = 3.0f;
		const int stepsCap = 200;        
		const float minBarW = 4.0f;       
		const float barWFracOfBox = 0.18f;

		const float halfW = boxHalfWidth;
		const float boxLeft = top.x - halfW;
		const float boxRight = top.x + halfW;
		const float boxH = fabsf(bottom.y - top.y);
		if (boxH <= 0.f) return;
		const float fullW = halfW * 2.0f;

		const float pad = max(4.0f, fullW * 0.06f);

		float barW = fullW * barWFracOfBox;
		barW = clampf(barW, minBarW, max(minBarW, fullW * 0.45f));

		float actualBoxLeft = boxLeft;
		if (!topIsCenterX) actualBoxLeft = top.x; 
		float barRight = actualBoxLeft - pad;
		float barLeft = barRight - barW;
		float barTop = top.y;
		float barBot = bottom.y;
		float barH = barBot - barTop;
		if (barH <= 0.f) return;

		DrawBox(Vec2{ barLeft, barTop }, Vec2{ barRight, barBot }, ImColor(0.0f, 0.0f, 0.0f, 0.78f), true, rounding);

		int steps = (int)roundf(min((float)stepsCap, max(6.0f, barH)));
		float sliceH = barH / (float)steps;
		int filledSteps = (int)roundf((float)steps * frac);
		int startFillIndex = steps - filledSteps; 

		ImColor topColor = ImColor(60.0f / 255.0f, 220.0f / 255.0f, 60.0f / 255.0f, 1.0f);
		ImColor bottomColor = ImColor(220.0f / 255.0f, 40.0f / 255.0f, 40.0f / 255.0f, 1.0f);

		const float insetX = max(1.0f, barW * 0.08f);
		for (int i = 0; i < steps; ++i) {
			float sliceTop = barTop + i * sliceH;
			float sliceBot = sliceTop + sliceH;
			float t = (steps <= 1) ? 0.0f : (float)i / (float)(steps - 1); 
			if (i >= startFillIndex) {
				ImColor col = LerpColor(topColor, bottomColor, t);
				DrawBox(Vec2{ barLeft + insetX, sliceTop + 0.35f }, Vec2{ barRight - insetX, sliceBot - 0.35f }, col, false, 1.0f);
			}
			else {
				DrawBox(Vec2{ barLeft + insetX, sliceTop + 0.35f }, Vec2{ barRight - insetX, sliceBot - 0.35f }, ImColor(0.06f, 0.06f, 0.06f, 0.55f), false, 1.0f);
			}
		}

		DrawBox(Vec2{ barLeft - 0.6f, barTop - 0.6f }, Vec2{ barRight + 0.6f, barBot + 0.6f }, ImColor(0.0f, 0.0f, 0.0f, 0.86f), true, rounding, 1.0f);
	}
}

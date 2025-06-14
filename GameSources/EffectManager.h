/*!
@file EffectManager.h
@brief Effect縺ｪ縺ｩ
*/

#pragma once
#include "stdafx.h"


#include <Effekseer.h>
#include <EffekseerRendererDX11.h>


#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )


namespace basecross {
	//--------------------------------------------------------------------------------------
	///	@class	EffectManeger
	///	@brief	Effekseer繝ｩ繧､繝悶Λ繝ｪ繧剃ｽｿ逕ｨ縺励◆繧ｨ繝輔ぉ繧ｯ繝医・蜀咲函縲∫ｮ｡逅・∵緒逕ｻ繧定｡後≧繧ｯ繝ｩ繧ｹ
	//--------------------------------------------------------------------------------------
	class EffectManeger : public MultiParticle {

	/// @brief 隱ｭ縺ｿ霎ｼ繧薙□繧ｨ繝輔ぉ繧ｯ繝医Μ繧ｽ繝ｼ繧ｹ繧堤ｮ｡逅・☆繧九・繝・・ (繧ｭ繝ｼ: wstring, 蛟､: EffectRef)
	map<wstring, Effekseer::EffectRef> m_ResMap;

	/// @brief 再生中のエフェクトハンドル
	Effekseer::Handle handle;

	/// @brief Effekseer縺ｮ邂｡逅・・繝阪・繧ｸ繝｣繝ｼ
	Effekseer::ManagerRef m_Manager;

	/// @brief Effekseer縺ｮ謠冗判繝ｬ繝ｳ繝繝ｩ繝ｼ (DirectX11逕ｨ)
	EffekseerRendererDX11::RendererRef m_renderer;

	/// @brief 邨碁℃譎る俣
	float m_TotalTime;

	/// @brief 単一のエフェクトリソースへの参照 
	Effekseer::EffectRef m_Effect;
	public:

		//--------------------------------------------------------------------------------------
		/*!
		@brief	繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
		@param[in]	stage	繧ｲ繝ｼ繝縺ｮ繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医↑縺ｩ縲∝・譛溷喧縺ｫ蠢・ｦ√↑諠・ｱ繧呈戟縺､繧ｪ繝悶ず繧ｧ繧ｯ繝・
		*/
		//--------------------------------------------------------------------------------------
		EffectManeger(const shared_ptr<Stage>& stage);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	繝・せ繝医Λ繧ｯ繧ｿ
		@details	Effekseer髢｢騾｣縺ｮ繝ｪ繧ｽ繝ｼ繧ｹ繧定ｧ｣謾ｾ縺励∪縺吶・
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EffectManeger();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	蛻晄悄蛹門・逅・
		@details	Effekseer縺ｮ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｨ繝ｬ繝ｳ繝繝ｩ繝ｼ繧堤函謌舌＠縲∝渕譛ｬ逧・↑險ｭ螳壹ｒ陦後＞縺ｾ縺吶・
		@return	縺ｪ縺・
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	譖ｴ譁ｰ蜃ｦ逅・
		@details	豈弱ヵ繝ｬ繝ｼ繝蜻ｼ縺ｳ蜃ｺ縺輔ｌ縲・ffekseer繝槭ロ繝ｼ繧ｸ繝｣繝ｼ繧呈峩譁ｰ縺励※繧ｨ繝輔ぉ繧ｯ繝医・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧帝ｲ繧√∪縺吶・
		@return	縺ｪ縺・
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	謠冗判蜃ｦ逅・・
		@details	豈弱ヵ繝ｬ繝ｼ繝蜻ｼ縺ｳ蜃ｺ縺輔ｌ縲∫ｮ｡逅・＠縺ｦ縺・ｋ繧ｨ繝輔ぉ繧ｯ繝医ｒ謠冗判縺励∪縺吶・
		@return	縺ｪ縺・
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	邨ゆｺ・・逅・
		@details	繧ｪ繝悶ず繧ｧ繧ｯ繝医・遐ｴ譽・凾縺ｫ縲∫｢ｺ菫昴＠縺溘Μ繧ｽ繝ｼ繧ｹ繧定ｧ｣謾ｾ縺励∪縺吶・
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDestroy()override;

		/*!
		@brief	bsm::Mat4x4 縺九ｉ Effekseer::Matrix44 縺ｸ陦悟・繧貞､画鋤縺励∪縺吶・
		@param[in]	src		螟画鋤蜈・・陦悟・ (bsm::Mat4x4)
		@param[out]	dest	螟画鋤蜈医・陦悟・ (Effekseer::Matrix44)
		*/
		void Mat4x4ToMatrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest);

		/*!
		@brief	繝薙Η繝ｼ陦悟・縺ｨ繝励Ο繧ｸ繧ｧ繧ｯ繧ｷ繝ｧ繝ｳ陦悟・繧偵Ξ繝ｳ繝繝ｩ繝ｼ縺ｫ險ｭ螳壹＠縺ｾ縺吶・
		@param[in]	view	繝薙Η繝ｼ陦悟・
		@param[in]	proj	繝励Ο繧ｸ繧ｧ繧ｯ繧ｷ繝ｧ繝ｳ陦悟・
		*/
		void SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj);

		/*!
		@brief	謖・ｮ壹＠縺溘お繝輔ぉ繧ｯ繝医・蜀咲函騾溷ｺｦ繧定ｨｭ螳壹＠縺ｾ縺吶・
		@param[in]	handle	蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@param[in]	speed	蜀咲函騾溷ｺｦ (1.0f縺梧ｨ呎ｺ夜・
		*/
		void SetEffectSpeed(Effekseer::Handle& handle, const float& speed);

		/*!
		@brief	繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺檎ｮ｡逅・☆繧句・縺ｦ縺ｮ繧ｨ繝輔ぉ繧ｯ繝医・譖ｴ譁ｰ繧剃ｸ譎ょ●豁｢/蜀埼幕縺励∪縺吶・
		@param[in]	pause	true縺ｧ荳譎ょ●豁｢縲’alse縺ｧ蜀埼幕
		*/
		void SetEffectPause(const bool& pause);

		/*!
		@brief	謖・ｮ壹＠縺溘く繝ｼ縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ｒ蜀咲函縺励∪縺吶・
		@param[out]	handle	蜀咲函縺輔ｌ縺溘お繝輔ぉ繧ｯ繝医・繝上Φ繝峨Ν縺後％縺ｮ蠑墓焚縺ｫ譬ｼ邏阪＆繧後∪縺吶・
		@param[in]	Key		繝ｪ繧ｽ繝ｼ繧ｹ繝槭ャ繝励↓逋ｻ骭ｲ縺励◆繧ｨ繝輔ぉ繧ｯ繝医・繧ｭ繝ｼ
		@param[in]	Emitter	繧ｨ繝輔ぉ繧ｯ繝医・逋ｺ逕溷ｺｧ讓・
		@param[in]	freme	繧ｨ繝輔ぉ繧ｯ繝医・髢句ｧ九ヵ繝ｬ繝ｼ繝 (0繝輔Ξ繝ｼ繝逶ｮ縺九ｉ蜀咲函縺吶ｋ蝣ｴ蜷医・0.0f)
		*/
		void PlayEffect(Effekseer::Handle& handle, const wstring& Key, const bsm::Vec3& Emitter, const float freme);

		/*!
		@brief	Effekseer縺ｮ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｨ繝ｬ繝ｳ繝繝ｩ繝ｼ繧堤函謌舌＠縲∝・譛溯ｨｭ螳壹ｒ陦後＞縺ｾ縺吶・
		*/
		void CreateEffectInterface();

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝医ヵ繧｡繧､繝ｫ(.efkefc)繧定ｪｭ縺ｿ霎ｼ縺ｿ縲√Μ繧ｽ繝ｼ繧ｹ繝槭ャ繝励↓逋ｻ骭ｲ縺励∪縺吶・
		@param[in]	Key			逋ｻ骭ｲ縺吶ｋ繧ｭ繝ｼ
		@param[in]	FileName	繧ｨ繝輔ぉ繧ｯ繝医ヵ繧｡繧､繝ｫ縺ｮ繝代せ
		*/
		void RegisterResource(const wstring& Key, const  wstring& FileName);

		/*!
		@brief	逋ｻ骭ｲ貂医∩縺ｮ繧ｨ繝輔ぉ繧ｯ繝医Μ繧ｽ繝ｼ繧ｹ繧貞叙蠕励＠縺ｾ縺吶・
		@param[in]	Key		蜿門ｾ励＠縺溘＞繧ｨ繝輔ぉ繧ｯ繝医・繧ｭ繝ｼ
		@return	Effekseer::EffectRef	繧ｨ繝輔ぉ繧ｯ繝医Μ繧ｽ繝ｼ繧ｹ縺ｸ縺ｮ蜿ら・縲りｦ九▽縺九ｉ縺ｪ縺・ｴ蜷医・nullptr縲・
		*/
		Effekseer::EffectRef GetEffectResource(const wstring& Key) const;

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝医・蠎ｧ讓吶ｒ逶ｸ蟇ｾ逧・↓遘ｻ蜍輔＆縺帙∪縺吶・
		@param[in]	handle		蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@param[in]	Location	迴ｾ蝨ｨ縺ｮ蠎ｧ讓吶↓蜉邂励☆繧狗ｧｻ蜍暮㍼
		*/
		void AddLocation(Effekseer::Handle& handle, const bsm::Vec3& Location);

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝医・蠎ｧ讓吶ｒ邨ｶ蟇ｾ蠎ｧ讓吶〒險ｭ螳壹＠縺ｾ縺吶・
		@param[in]	handle		蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@param[in]	Location	險ｭ螳壹☆繧句ｺｧ讓・
		*/
		void SetLocation(Effekseer::Handle& handle, const bsm::Vec3& Location);

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝医・蝗櫁ｻ｢繧定ｻｸ縺ｨ隗貞ｺｦ縺ｧ險ｭ螳壹＠縺ｾ縺吶・
		@param[in]	handle		蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@param[in]	Rotation	蝗櫁ｻ｢霆ｸ
		@param[in]	angle		蝗櫁ｻ｢隗貞ｺｦ (繝ｩ繧ｸ繧｢繝ｳ)
		*/
		void SetRotation(Effekseer::Handle& handle, const bsm::Vec3& Rotation, const float angle);

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝医・諡｡螟ｧ邵ｮ蟆冗紫繧定ｨｭ螳壹＠縺ｾ縺吶・
		@param[in]	handle	蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@param[in]	Scale	XYZ蜷・ｻｸ縺ｮ諡｡螟ｧ邇・
		*/
		void SetScale(Effekseer::Handle& handle, const bsm::Vec3& Scale);

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝亥・菴薙・濶ｲ繧定ｨｭ螳壹＠縺ｾ縺吶・
		@param[in]	handle	蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@param[in]	color	險ｭ螳壹☆繧玖牡 (RGBA)
		*/
		void SetAllColor(Effekseer::Handle& handle, const bsm::Col4 color);

		/*!
		@brief	謖・ｮ壹＠縺溘お繝輔ぉ繧ｯ繝医・蠑ｷ蛻ｶ遐ｴ譽・ｒ縺励∪縺吶・
		@param[in]	handle	蛛懈ｭ｢縺吶ｋ繧ｨ繝輔ぉ繧ｯ繝医・繝上Φ繝峨Ν
		*/
		void StopEffect(Effekseer::Handle& handle);

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝医・謠冗判繝ｬ繧､繝､繝ｼ繧定ｨｭ螳壹＠縺ｾ縺吶・
		@details	繝ｬ繧､繝､繝ｼ縺斐→縺ｫ謠冗判繧ｿ繧､繝溘Φ繧ｰ繧貞宛蠕｡縺励◆縺・ｴ蜷医↑縺ｩ縺ｫ菴ｿ逕ｨ縺励∪縺吶・
		@param[in]	handle	蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@param[in]	layer	險ｭ螳壹☆繧九Ξ繧､繝､繝ｼ逡ｪ蜿ｷ
		*/
		void SetLayer(Effekseer::Handle& handle, int32_t layer);

		/*!
		@brief	繧ｨ繝輔ぉ繧ｯ繝医・謠冗判繝ｬ繧､繝､繝ｼ繧貞叙蠕励＠縺ｾ縺吶・
		@param[in]	handle	蟇ｾ雎｡縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν
		@return	繝ｬ繧､繝､繝ｼ逡ｪ蜿ｷ
		*/
		int32_t GetLayer(Effekseer::Handle& handle)
		{
			return m_Manager->GetLayer(handle);
		}
	};
}
//end basecross

#include "ShaderManager.h"

ShaderManager* ShaderManager::Getinstance()
{
	static ShaderManager instance;
	return &instance;
}

void ShaderManager::Initialize()
{
	DxcCreate();
	includeHandlerSetting();
	ShaderComples();
}

IDxcBlob* ShaderManager::CompilerShaderFanc(const std::wstring& filePath, const wchar_t* profile)
{
	ComPtr<IDxcUtils> dxcUtils = ShaderManager::Getinstance()->dxc.m_pUtils.Get();
	ComPtr<IDxcCompiler3> dxcCompiler = ShaderManager::Getinstance()->dxc.m_pCompiler.Get();
	ComPtr<IDxcIncludeHandler> includeHandler = ShaderManager::Getinstance()->dxc.m_pIncludeHandler.Get();

	//1.hlslファイルを読む
	LogManager::Log(LogManager::ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr =
		dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//2.Compileする
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, arguments, _countof(arguments), includeHandler.Get(), IID_PPV_ARGS(&shaderResult));
	//コンパイルエラーではなくdxcが起動出来ないなど致命的な状況
	assert(SUCCEEDED(hr));

	//3.警告・エラーが出ていないかを確認する
	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		LogManager::Log(shaderError->GetStringPointer());
		assert(false);
	}
	//4.Compile結果を受け取って返す
	//BLOB・・・BinaryLargeOBject
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	LogManager::Log(LogManager:: ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;

}


void ShaderManager::DxcCreate()
{
	HRESULT hr = {};

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&ShaderManager::Getinstance()->dxc.m_pUtils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&ShaderManager::Getinstance()->dxc.m_pCompiler));
	assert(SUCCEEDED(hr));
}

void ShaderManager::includeHandlerSetting()
{
	ShaderManager::Getinstance()->dxc.m_pUtils->CreateDefaultIncludeHandler(&ShaderManager::Getinstance()->dxc.m_pIncludeHandler);
}

void ShaderManager::ShaderComples()
{
	ShapeShader();
	SpriteShader();
	LightShader();
}

void ShaderManager::ShapeShader()
{
	SShaderMode shaders; shaders;
	shaders.vertexBlob =
		ShaderManager::CompilerShaderFanc(
			L"Resources/Shader/ShapeObject3d.VS.hlsl",
			L"vs_6_0");
	shaders.pixelBlob =
		ShaderManager::CompilerShaderFanc(
			L"Resources/Shader/ShapeObject3d.PS.hlsl",
			L"ps_6_0");
	ShaderManager::Getinstance()->shaders_.shape = shaders;
}

void ShaderManager::SpriteShader()
{
	SShaderMode shaders;
	shaders.vertexBlob =
		ShaderManager::CompilerShaderFanc(
			L"Resources/Shader/SpriteObject3d.VS.hlsl",
			L"vs_6_0");

	shaders.pixelBlob =
		ShaderManager::CompilerShaderFanc(
			L"Resources/Shader/SpriteObject3d.PS.hlsl",
			L"ps_6_0");
	ShaderManager::Getinstance()->shaders_.sprite = shaders;
}

void ShaderManager::LightShader()
{
	SShaderMode shaders;
	shaders.vertexBlob =
		ShaderManager::CompilerShaderFanc(
			L"Resources/Shader/Harf_LambertLightObject.VS.hlsl",
			L"vs_6_0");

	shaders.pixelBlob =
		ShaderManager::CompilerShaderFanc(
			L"Resources/Shader/Harf_LambertLightObject.PS.hlsl",
			L"ps_6_0");
	ShaderManager::Getinstance()->shaders_.light = shaders;


}


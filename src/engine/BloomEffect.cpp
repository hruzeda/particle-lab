#include "../../inc/engine/BloomEffect.h"

BloomEffect::BloomEffect()
    : mShaders(),
      mBrightnessTexture(),
      mFirstPassTextures(),
      mSecondPassTextures() {
  mShaders.load(Shaders::BrightnessPass, "media/Shaders/Fullpass.vert",
                "media/Shaders/Brightness.frag");
  mShaders.load(Shaders::DownSamplePass, "media/Shaders/Fullpass.vert",
                "media/Shaders/DownSample.frag");
  mShaders.load(Shaders::GaussianBlurPass, "media/Shaders/Fullpass.vert",
                "media/Shaders/GuassianBlur.frag");
  mShaders.load(Shaders::AddPass, "media/Shaders/Fullpass.vert",
                "media/Shaders/Add.frag");
}

void BloomEffect::apply(const sf::RenderTexture& input,
                        sf::RenderTarget& output) {
  prepareTextures(input.getSize());

  filterBright(input, mBrightnessTexture);

  downsample(mBrightnessTexture, mFirstPassTextures[0]);
  blurMultipass(mFirstPassTextures);

  downsample(mFirstPassTextures[0], mSecondPassTextures[0]);
  blurMultipass(mSecondPassTextures);

  add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
  mFirstPassTextures[1].display();
  add(input, mFirstPassTextures[1], output);
}

void BloomEffect::prepareTextures(sf::Vector2u size) {
  if (mBrightnessTexture.getSize() != size) {
    mBrightnessTexture.create(size.x, size.y);
    mBrightnessTexture.setSmooth(true);

    mFirstPassTextures[0].create(size.x / 2, size.y / 2);
    mFirstPassTextures[0].setSmooth(true);
    mFirstPassTextures[1].create(size.x / 2, size.y / 2);
    mFirstPassTextures[1].setSmooth(true);

    mSecondPassTextures[0].create(size.x / 4, size.y / 4);
    mSecondPassTextures[0].setSmooth(true);
    mSecondPassTextures[1].create(size.x / 4, size.y / 4);
    mSecondPassTextures[1].setSmooth(true);
  }
}

void BloomEffect::filterBright(const sf::RenderTexture& input,
                               sf::RenderTexture& output) {
  sf::Shader& brightness = mShaders.get(Shaders::BrightnessPass);

  brightness.setParameter("source", input.getTexture());
  applyShader(brightness, output);
  output.display();
}

void BloomEffect::blurMultipass(RenderTextureArray& renderTextures) {
  sf::Vector2u textureSize = renderTextures[0].getSize();

  for (std::size_t count = 0; count < 2; ++count) {
    blur(renderTextures[0], renderTextures[1],
         sf::Vector2f(0.f, 1.f / textureSize.y));
    blur(renderTextures[1], renderTextures[0],
         sf::Vector2f(1.f / textureSize.x, 0.f));
  }
}

void BloomEffect::blur(const sf::RenderTexture& input,
                       sf::RenderTexture& output, sf::Vector2f offsetFactor) {
  sf::Shader& gaussianBlur = mShaders.get(Shaders::GaussianBlurPass);

  gaussianBlur.setParameter("source", input.getTexture());
  gaussianBlur.setParameter("offsetFactor", offsetFactor);
  applyShader(gaussianBlur, output);
  output.display();
}

void BloomEffect::downsample(const sf::RenderTexture& input,
                             sf::RenderTexture& output) {
  sf::Shader& downSampler = mShaders.get(Shaders::DownSamplePass);

  downSampler.setParameter("source", input.getTexture());
  downSampler.setParameter("sourceSize", sf::Vector2f(input.getSize()));
  applyShader(downSampler, output);
  output.display();
}

void BloomEffect::add(const sf::RenderTexture& source,
                      const sf::RenderTexture& bloom,
                      sf::RenderTarget& output) {
  sf::Shader& adder = mShaders.get(Shaders::AddPass);

  adder.setParameter("source", source.getTexture());
  adder.setParameter("bloom", bloom.getTexture());
  applyShader(adder, output);
}

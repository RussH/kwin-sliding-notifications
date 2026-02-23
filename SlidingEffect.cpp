#include <effect/effect.h>
#include <effect/effectwindow.h>
#include <effect/animationeffect.h>
#include <effect/effecthandler.h>
#include <KPluginFactory>
#include <QEasingCurve>
#include <QElapsedTimer>
#include <QMap>
#include <QProcess>
#include <QDebug>

using namespace Qt::StringLiterals;

namespace KWin {

    class SlidingEffect : public AnimationEffect {
        Q_OBJECT
    public:
        explicit SlidingEffect(QObject *parent = nullptr, const QVariantList &args = QVariantList())
        : AnimationEffect() {
            Q_UNUSED(parent);
            Q_UNUSED(args);

            // LOG FOR VERIFICATION
            qWarning() << "!!! [SLIDE-PLUGIN-LOADED] VERSION: CACHY-PLASMA-6.6-V12 !!!";

            connect(effects, &EffectsHandler::windowAdded, this, &SlidingEffect::onWindowAdded);
            connect(effects, &EffectsHandler::windowClosed, this, &SlidingEffect::onWindowClosed);
            connect(effects, &EffectsHandler::windowDeleted, this, &SlidingEffect::onWindowDeleted);
        }

        bool isNotification(EffectWindow *w) {
            if (!w) return false;
            if (w->isNotification()) return true;

            // Check for Plasma 6 specific classes
            bool isPlasma = w->windowClass().contains(u"plasmashell"_s, Qt::CaseInsensitive);
            bool isNotifyRole = w->windowRole().contains(u"notification"_s, Qt::CaseInsensitive);

            return (isPlasma && isNotifyRole);
        }

        void onWindowAdded(EffectWindow *w) {
            if (isNotification(w)) {
                // PLAY SOUND: Using pw-play for PipeWire (Standard on CachyOS)
                QProcess::startDetached(u"/usr/bin/pw-play"_s, {u"/usr/share/sounds/ocean/stereo/message-attention.oga"_s});

                // Intro Animation: Slide in from right (400px) and fade in
                animate(w, AnimationEffect::Translation, 0, 350, FPx2(0.0),
                        QEasingCurve::OutBack, 0, FPx2(400.0));
                animate(w, AnimationEffect::Opacity, 0, 250, FPx2(1.0),
                        QEasingCurve::Linear, 0, FPx2(0.0));
            }
        }

        void onWindowClosed(EffectWindow *w) {
            if (isNotification(w)) {
                QElapsedTimer *timer = new QElapsedTimer();
                timer->start();
                m_timers[w] = timer;

                // Keep window alive for the exit slide (500ms)
                animate(w, AnimationEffect::Opacity, 0, 500, FPx2(0.0),
                        QEasingCurve::Linear, 0, FPx2(1.0), false, true);
            }
        }

        void onWindowDeleted(EffectWindow *w) {
            if (m_timers.contains(w)) {
                delete m_timers.take(w);
            }
        }

        void drawWindow(const RenderTarget &renderTarget, const RenderViewport &viewport, EffectWindow *w, int mask, const Region &region, WindowPaintData &data) override {
            if (isNotification(w)) {
                // Ensure no vertical/depth jitter
                data.setYTranslation(0);
                data.setZTranslation(0);

                if (m_timers.contains(w)) {
                    qint64 elapsed = m_timers[w]->elapsed();

                    // EXIT SHIELD:
                    // If the animation is over (200ms) OR if it's moved even a tiny bit,
                    // force it to invisible. This stops the "pop-back" frame.
                    if (elapsed > 200) {
                        data.setOpacity(0.0);
                        return;
                    }

                    float progress = static_cast<float>(elapsed) / 200.0f;
                    data.setXTranslation(500.0f * progress);
                    data.setOpacity(1.0f - progress);
                }
            }
            // Pass the new Region type up to the parent
            AnimationEffect::drawWindow(renderTarget, viewport, w, mask, region, data);
        }

    private:
        QMap<EffectWindow*, QElapsedTimer*> m_timers;
    };

} // namespace KWin

KWIN_EFFECT_FACTORY_SUPPORTED(KWin::SlidingEffect, "kwin_final_sliding.json", return true;)

#include "SlidingEffect.moc"

using System.Linq;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

namespace SwgohRestored.Battle
{
    public sealed class OriginalTrainingNotice : MonoBehaviour
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        static void DirectDemo()
        {
            if (System.Environment.GetCommandLineArgs().Contains("--vader-demo") && SceneManager.GetActiveScene().name != "BattleDemo")
                SceneManager.LoadScene("BattleDemo");
        }
        void Start()
        {
            var canvasObject = new GameObject("TrainingScope", typeof(Canvas), typeof(CanvasScaler));
            canvasObject.transform.SetParent(transform, false);
            var canvas = canvasObject.GetComponent<Canvas>(); canvas.renderMode = RenderMode.ScreenSpaceOverlay; canvas.sortingOrder = 100;
            var scaler = canvasObject.GetComponent<CanvasScaler>(); scaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize; scaler.referenceResolution = new Vector2(1920,1080);
            var obj = new GameObject("TrainingScopeText", typeof(RectTransform), typeof(Text)); obj.transform.SetParent(canvasObject.transform, false);
            var label = obj.GetComponent<Text>(); label.font = SwgohRestored.UI.UiKit.FontCjk();
            label.fontSize = 16; label.color = new Color(1,.84f,.48f); label.raycastTarget = false;
            label.horizontalOverflow = HorizontalWrapMode.Overflow;
            label.verticalOverflow = VerticalWrapMode.Overflow;
            label.text = "维达技能管线训练（Demo）｜队长技能未启用｜部分特效与音效待恢复";
            var rect = obj.GetComponent<RectTransform>(); rect.anchorMin = rect.anchorMax = rect.pivot = new Vector2(0,1);
            rect.anchoredPosition = new Vector2(20,-180); rect.sizeDelta = new Vector2(1100,60);
        }
    }
}

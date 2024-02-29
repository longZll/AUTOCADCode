using System.Collections.Generic;
using System.Linq;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Runtime;
using DotNetARX;
namespace DynBlock
{
    public class DynBlock
    {
        [CommandMethod("CountDynBlock")]
        public void CountDynBlock()
        {
            Database db=HostApplicationServices.WorkingDatabase;
            using (Transaction trans=db.TransactionManager.StartTransaction())
            {
                //选取模型空间中的Door动态块，并限定其Flip vertical属性为1（向右开的门）
                var doors=from d in db.GetEntsInModelSpace<BlockReference>()
                          where d.GetBlockName() == "Door" && d.ObjectId.GetDynBlockValue("Flip vertical") == "1"
                          select d;
                //显示向右开的门的个数
                Application.ShowAlertDialog("向右开的门共有" + doors.Count() + "个");
                trans.Commit();
            }
        }

        [CommandMethod("InsertDynBlock")]
        public void InsertDynBlock()
        {
            Database db=HostApplicationServices.WorkingDatabase;
            using (Transaction trans=db.TransactionManager.StartTransaction())
            {
                ObjectId model=db.GetModelSpaceId();//获取模型空间的ObjectId
                //表示块静态属性的字典对象
                Dictionary<string,string> atts=new Dictionary<string, string>();
                //添加门的各种静态属性
                atts.Add("SYM.", "2");
                atts.Add("WIDTH", "0.8m");
                atts.Add("HEIGHT", "2m");
                atts.Add("STYLE", "ONE PANEL");
                atts.Add("REF#", "TS 1040");
                atts.Add("MANUFACTURE", "TUR STYLE");
                atts.Add("COST", "200.00");
                //在模型空间的Doors层上插入表示门的Door块，插入点为原点，不旋转，不缩放
                ObjectId blockId=model.InsertBlockReference("Doors", "Door", Point3d.Origin, new Scale3d(), 0, atts);
                //设置门的动态属性，宽度为0.8个单位，垂直方向翻转
                blockId.SetDynBlockValue("Door Width", 0.8);
                blockId.SetDynBlockValue("Flip vertical", 1);
                trans.Commit();
            }
        }
    }
}

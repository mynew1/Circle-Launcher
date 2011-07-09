local frame = CreateFrame("FRAME");
frame:RegisterEvent("ADDON_LOADED");

function frame:OnEvent(event, arg1)
    if event == "ADDON_LOADED" and arg1 == "CharData" then
        if CharactersData == nil then
            CharactersData = {};
        end

        CharactersData[GetRealmName().."@"..UnitName("player").."-name"]    = UnitName("player");
        CharactersData[GetRealmName().."@"..UnitName("player").."-level"]   = UnitLevel("player");
        CharactersData[GetRealmName().."@"..UnitName("player").."-race"]    = UnitRace("player");
        CharactersData[GetRealmName().."@"..UnitName("player").."-class"]   = UnitClass("player");
        CharactersData[GetRealmName().."@"..UnitName("player").."-lastIn"]  = date("%m/%d/%y %H:%M:%S");
	CharactersData[GetRealmName().."@"..UnitName("player").."-sex"]     = UnitSex("player");

    end
end

frame:SetScript("OnEvent", frame.OnEvent);

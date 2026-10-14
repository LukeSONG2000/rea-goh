# SWGoH 客户端协议 DTO 清单

来源：global-metadata.dat v39（`metadata39.py` 解析，GameAssembly.dll 版本 0.40.7）。
范围：命名空间 `Protos`、`Rpc`，共 **1243** 个类型。

字段仅记录**名称**（字段类型在 GameAssembly.dll 的 Il2CppType 表里，需 dump.cs 阶段才有）。

| 分类 | 数量 |
|---|---|
| other | 572 |
| Request | 238 |
| Response | 224 |
| enum | 170 |
| Event | 33 |
| Command | 6 |

## 类型明细

| 类型 | 字段数 | 字段 |
|---|---|---|
| `Protos.AbandonChallengeRequest` | 2 | ChallengeId, extensionObject |
| `Protos.AbandonChallengeResponse` | 1 | extensionObject |
| `Protos.Ability` | 33 | Id, NameKey, DescKey, PrefabName, _triggerCondition, StackingLineOverride, _tier, Cooldown, Icon, ApplyTypeTooltipKey, _descriptiveTag, _effectReference, ConfirmationMessage, ButtonLocation, ShortDescKey, AbilityType, DetailLocation, AllyTargetingRuleId, UseAsReinforcementDesc, _interactsWithTag, SubIcon, AiParams, CooldownType, AlwaysDisplayInBattleUi, … (+9) |
| `Protos.AbilityAIParams` | 7 | PreferredAllyTargetingRuleId, PreferredEnemyTargetingRuleId, RequireEnemyPreferredTargets, RequireAllyTargets, _preferredAllyTargetingRuleIdList, _preferredEnemyTargetingRuleIdList, extensionObject |
| `Protos.enum AbilityButtonLocationType` | 5 | value__, ABILITYSTANDARD, ABILITYADDITIONAL, ABILITYHIDDEN, ABILITYSECONDSTANDARD |
| `Protos.enum AbilityCooldownType` | 3 | value__, ABILITYCOOLDOWNTURNBASED, ABILITYCOOLDOWNSPEEDBASED |
| `Protos.AbilityDecisionNode` | 9 | DecisionType, DecisionTarget, _param, ParamModifier, TargetExcludeSelf, Chance, Negate, DecisionResult, extensionObject |
| `Protos.enum AbilityDecisionParamModifier` | 4 | value__, GREATERTHAN, EQUALTO, LESSTHAN |
| `Protos.AbilityDecisionResult` | 4 | ChosenAbilityTag, DecisionResultTarget, _allyTargetId, extensionObject |
| `Protos.enum AbilityDecisionResultTarget` | 3 | value__, ALL, LOWESTABSOLUTEHEALTH |
| `Protos.enum AbilityDecisionTarget` | 5 | value__, SELF, ALLY, ENEMY, DEADALLY |
| `Protos.AbilityDecisionTree` | 3 | AbilityDecisionTreeId, _abilityDecisionNode, extensionObject |
| `Protos.enum AbilityDecisionType` | 13 | value__, SINGLEHEALTH, MEDIANHEALTH, NUMBEROFTARGETS, PERSISTENTEFFECT, NUMBEROFENEMIESALIVE, REINFORCEAVAILABLE, OUTNUMBERED, SINGLESURVIVABILITYPERCENT, SUMMONAVAILABLE, SINGLEPROTECTION, SINGLESPEEDFILL, TARGETBYUNITIDS |
| `Protos.AbilityModifierData` | 5 | BattleSide, Tint, TierKey, Icon, extensionObject |
| `Protos.AbilityReference` | 13 | AbilityId, RequiredTier, RequiredRarity, RequiredSkillTier, RequiredRelicTier, OverrideAlwaysDisplayInBattleUi, OverrideIcon, OverrideNameKey, OverrideDescKey, PowerAdditiveTag, UnlockRecipeId, InheritMappingId, extensionObject |
| `Protos.AbilitySynergy` | 3 | _separateCategoryId, _groupedCategoryId, extensionObject |
| `Protos.AbilityTier` | 8 | DescKey, UpgradeDescKey, CooldownMaxOverride, _effectReference, _interactsWithTag, BlockingEffectId, BlockedLocKey, extensionObject |
| `Protos.enum AbilityType` | 6 | value__, ABILITYACTIVATED, ABILITYPASSIVE, ABILITYREINFORCEMENT, ABILITYACTIVATEDALLYSELECT, ABILITYHARDWARE |
| `Protos.enum AcceptEncoding` | 3 | value__, DEFAULTACCEPTENCODING, GZIPACCEPTENCODING |
| `Protos.AcceptEulaRequest` | 1 | extensionObject |
| `Protos.AcceptEulaResponse` | 1 | extensionObject |
| `Protos.AcceptJoinGuildRequest` | 2 | PlayerId, extensionObject |
| `Protos.AcceptJoinGuildResponse` | 4 | _status, Member, Profile, extensionObject |
| `Protos.AcceptRerolledModRequest` | 2 | KeepMod, extensionObject |
| `Protos.AcceptRerolledModResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.AccountLinkCodeRequest` | 1 | extensionObject |
| `Protos.AccountLinkCodeResponse` | 3 | SourceLinkCode, ExpireTime, extensionObject |
| `Protos.ActionLink` | 3 | Link, Type, extensionObject |
| `Protos.enum ActionLinkType` | 3 | value__, INTERNAL, EXTERNAL |
| `Protos.ActivateChallengeRequest` | 2 | ChallengeId, extensionObject |
| `Protos.ActivateChallengeResponse` | 1 | extensionObject |
| `Protos.AddModFilterRequest` | 2 | Filter, extensionObject |
| `Protos.AddModFilterResponse` | 1 | extensionObject |
| `Protos.AddModLoadoutTabRequest` | 2 | TabName, extensionObject |
| `Protos.AddModLoadoutTabResponse` | 2 | ModLoadoutTabAdded, extensionObject |
| `Protos.AddSquadPresetTabRequest` | 3 | CombatType, TabName, extensionObject |
| `Protos.AddSquadPresetTabResponse` | 2 | SquadPresetTabAdded, extensionObject |
| `Protos.AdditionalItemModifier` | 2 | _addedItem, extensionObject |
| `Protos.AgeGateStatus` | 7 | Age, PassesCoppa, CanInvite, CanRequestUserEmail, ConfirmInviteEmail, LastAuthService, extensionObject |
| `Protos.Ally` | 21 | AllyId, AllyName, AllyStatus, Timestamp, AllyCode, CharacterCombatLeader, LastUsedInBattle, PlayerLevel, ArenaRank, ArenaWins, ArenaLosses, CompletedMissions, Guild, LastActivityTime, AllyTitle, AllyPortrait, _seasonStatus, LifetimeSeasonScore, LeagueId, NucleusId, extensionObject |
| `Protos.AllyChangeRequest` | 2 | _allyId, extensionObject |
| `Protos.AllyChangeResponse` | 2 | _ally, extensionObject |
| `Protos.AllyGuildPreview` | 6 | GuildId, GuildName, GuildBannerColorId, GuildBannerLogoId, GuildTypeId, extensionObject |
| `Protos.AllyList` | 3 | _ally, MaxSize, extensionObject |
| `Protos.AllyListRequest` | 1 | extensionObject |
| `Protos.AllyListResponse` | 2 | AllyList, extensionObject |
| `Protos.enum AllyStatus` | 3 | value__, ALLIED, REQUESTSENT |
| `Protos.ApplyLightspeedTokenRequest` | 3 | LightspeedTokenId, UnitDefinitionId, extensionObject |
| `Protos.ApplyLightspeedTokenResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.ArenaOverviewData` | 7 | CharacterArenaRank, ShipArenaRank, _awayDefendedZone, _homeDefendedZone, HomeScore, AwayScore, extensionObject |
| `Protos.Artifact` | 4 | Id, DefinitionId, Timestamp, extensionObject |
| `Protos.ArtifactDefinition` | 9 | Id, PowerLevel, _grantedAbility, NameKey, DescriptionKey, Texture, _targetedStat, TierId, extensionObject |
| `Protos.ArtifactTierDefinition` | 5 | Id, Rarity, NameKey, IconKey, extensionObject |
| `Protos.AttackRange` | 5 | StartInclusive, EndInclusive, AbsoluteRange, RelativeRange, extensionObject |
| `Protos.AuthByAccessCodeRequest` | 6 | AccessCode, ClientIpAddress, RedirectUri, PageContext, CodeVerifier, extensionObject |
| `Protos.AuthByAccessCodeResponse` | 3 | SessionId, SessionToken, extensionObject |
| `Protos.AuthByRefreshTokenRequest` | 1 | extensionObject |
| `Protos.AuthByRefreshTokenResponse` | 5 | SessionId, SessionToken, RefreshToken, RefreshTokenExpiryUtcSeconds, extensionObject |
| `Protos.AuthGameCenterRequest` | 12 | GameCenterId, Language, BindOption, BundleId, PlayerName, GuestId, LocalTimeZoneOffsetMinutes, SessionStartContext, DevicePreferences, IdentityVerificationData, StorePlatform, extensionObject |
| `Protos.AuthGameCenterResponse` | 4 | AuthId, AuthToken, BindChallenge, extensionObject |
| `Protos.AuthGoogleRequest` | 12 | OauthToken, Language, BindOption, BundleId, PlayerName, GuestId, LocalTimeZoneOffsetMinutes, SessionStartContext, DevicePreferences, AuthCode, StorePlatform, extensionObject |
| `Protos.AuthGoogleResponse` | 4 | AuthId, AuthToken, BindChallenge, extensionObject |
| `Protos.AuthGuestRequest` | 9 | Uid, Language, PlayerName, BundleId, LocalTimeZoneOffsetMinutes, SessionStartContext, DevicePreferences, StorePlatform, extensionObject |
| `Protos.AuthGuestResponse` | 3 | AuthId, AuthToken, extensionObject |
| `Protos.AuthLogoutRequest` | 1 | extensionObject |
| `Protos.AuthLogoutResponse` | 1 | extensionObject |
| `Protos.AuthNucleusRequest` | 12 | NucleusAuthToken, Language, BindOption, BundleId, PlayerName, GuestId, LocalTimeZoneOffsetMinutes, SessionStartContext, DevicePreferences, CreatePlayerIfNotBound, StorePlatform, extensionObject |
| `Protos.AuthNucleusResponse` | 4 | AuthId, AuthToken, BindChallenge, extensionObject |
| `Protos.enum AuthServiceType` | 7 | value__, UNKNOWNAUTH, GUESTAUTH, GOOGLEPLAYAUTH, GAMECENTERAUTH, GOOGLEGAMESAUTH, NUCLEUSAUTH |
| `Protos.AuthSessionActiveRequest` | 1 | extensionObject |
| `Protos.AuthSessionActiveResponse` | 2 | Active, extensionObject |
| `Protos.BatchLevelModRequest` | 4 | _modIds, ModLevel, ModSessionId, extensionObject |
| `Protos.BattleAbilityExecutionStat` | 3 | AbilityDescriptiveTag, ExecutionCount, extensionObject |
| `Protos.BattleAction` | 3 | RecordIndex, BattleMoveRecord, extensionObject |
| `Protos.BattleCondition` | 5 | DefaultHash1, DefaultHash2, ConditionType, ConditionValue, Priority |
| `Protos.BattleConsumeRecord` | 3 | ConsumeId, _targetUnitId, extensionObject |
| `Protos.BattleCriteriaStats` | 27 | _damageToStat, _damageFromStat, _effectStat, _abilityStat, _unitStat, _killStat, _deathStat, SquadSize, MaxSquadSize, _healingFromStat, BattleDurationInSeconds, BattleTurnCount, PlayerReinforcementsDeployed, _playerUltimateAbilityStat, _enemyUltimateAbilityStat, _playerCritCountStat, PlayerSpecialAbilityCount, PlayerSuccessfulEvasions, PlayerCritStreak, PlayerOutOfTurnMoves, PlayerTurnMeterRemoval, PlayerMostEnemiesDefeatedPerTurn, _playerCategoryTurnStreak, _playerCategoryTurnStreakBeforeEnemyTurn, … (+3) |
| `Protos.BattleData` | 8 | BattleSeed, _battleEncounter, _completionReward, TargetingSeed, EncounterIndex, _units, _grantedAbility, extensionObject |
| `Protos.enum BattleDeploymentState` | 4 | value__, BATTLEDEPLOYMENTSTATEANY, BATTLEDEPLOYMENTSTATEDEPLOYED, BATTLEDEPLOYMENTSTATEREINFORCEMENT |
| `Protos.BattleDetails` | 12 | Id, EntryCategoryAllowed, _battleEncounter, BattleDurationLimit, _grantedAbility, AutoBattleEnabled, BattleSpeedAdjustable, RetreatEnabled, AudioKey, CombatType, PveDatacronAllowed, extensionObject |
| `Protos.BattleDistributionRecord` | 1 | extensionObject |
| `Protos.BattleDropRecord` | 5 | _lootDrop, LootReason, SourceId, TargetId, extensionObject |
| `Protos.BattleEffectExecutionStat` | 3 | EffectDescriptiveTag, ExecutionCount, extensionObject |
| `Protos.BattleEncounter` | 5 | Squad, _unitState, EncounterId, CurrentEncounterScore, extensionObject |
| `Protos.BattleEnvironment` | 3 | Prefab, _audioPackage, extensionObject |
| `Protos.BattleFinishEvent` | 3 | BattleFinishRequest, MatchedPlayer, extensionObject |
| `Protos.BattleFinishRaidScore` | 3 | EncounterId, RawEarnedScore, extensionObject |
| `Protos.BattleFinishRequest` | 13 | _battleRound, BattleState, BattleOutcome, IsEditor, BattleTimedOut, Type, _battleRecord, AppCrashRecovery, BattleStats, Progress, _raidScore, SubmitResults, extensionObject |
| `Protos.BattleFinishResponse` | 20 | _inventoryDelta, UpdatedRoster, PlayerUpdate, PreventAllyRequest, PushNotesPrompt, PushNotesEnabledIncentive, RaidStatus, RaidResult, GuildInventory, GuildProgress, PvpStatus, WarStatus, TerritoryBattleStatus, TerritoryWarStatus, TournamentMatchStatus, TournamentEventId, _unitStamina, ConquestNodeStatus, ColiseumBossStatus, extensionObject |
| `Protos.enum BattleLootReason` | 4 | value__, ONHIT, ONDEATH, COMPLETEREWARD |
| `Protos.BattleMoveRecord` | 16 | SourceUnitId, _target, Crit, EncounterCompleted, WeakAgainst, StrongAgainst, MoveType, HitSuccess, ApplyType, DamageType, SourceDamage, CritDamage, Mitigation, AppliedEffectId, AbilityId, extensionObject |
| `Protos.enum BattleOutcome` | 6 | value__, WIN, LOSS, RETREAT, DRAW, CLIENTSTOPPED |
| `Protos.BattleRecord` | 17 | RecordType, Timestamp, SourceUnitId, _targetUnitIds, AbilityId, EffectId, DamageAmount, RecoveryType, RecoveryAmount, IsCrit, AutoBattleOn, EncounterIndex, ExcessDamageAmount, ExcessRecoveryAmount, AllyUnitId, RecordIndex, extensionObject |
| `Protos.enum BattleRecordType` | 18 | value__, BATTLERECORDABILITY, BATTLERECORDCONDITION, BATTLERECORDDAMAGE, BATTLERECORDEFFECTAPPLIED, BATTLERECORDEFFECTEXPIRED, BATTLERECORDEFFECT, BATTLERECORDPLAYERAUTOTOGGLE, BATTLERECORDPLAYERMANUALABILITY, BATTLERECORDPLAYERMANUALSELECT, BATTLERECORDRECOVERY, BATTLERECORDENCOUNTERSTART, BATTLERECORDENCOUNTEREND, BATTLERECORDOPENINGMOVE, BATTLERECORDSELECTREINFORCEMENT, BATTLERECORDDESTROY, BATTLERECORDROUNDINCREMENTED, BATTLERECORDSUMMON |
| `Protos.BattleRound` | 2 | _battleTurn, extensionObject |
| `Protos.BattleStartEvent` | 8 | GamedataVersion, ClientVersion, BattleStartRequest, BattleData, PlayerSquad, TerritoryWarStatus, GuildRaidStatus, extensionObject |
| `Protos.BattleStartRequest` | 14 | CampaignMissionIdentifier, BattleSupportChosen, PlayerSquad, EventId, InstanceId, Type, OpponentId, ZoneId, KnownCommandState, TournamentMatchId, _playerDatacronTelemetry, _opponentDatacronTelemetry, RaidModifierIndex, extensionObject |
| `Protos.BattleStartResponse` | 6 | BattleData, _inventoryDelta, Player, TerritoryWarStatus, TournamentMatchStatus, extensionObject |
| `Protos.BattleState` | 6 | EncounterSequenceNumber, BattleData, _opponentState, _playerState, _playerSquadState, extensionObject |
| `Protos.BattleSupport` | 15 | Id, Name, Level, IsAlly, SupportReward, SupportUnit, GuildName, GuildBannerColorId, GuildBannerLogoId, PlayerTitle, PlayerPortrait, InStarterGuild, _seasonStatus, LeagueId, extensionObject |
| `Protos.BattleSupportRequest` | 4 | SquadType, CombatType, CampaignMissionIdentifier, extensionObject |
| `Protos.BattleSupportResponse` | 2 | _battleSupport, extensionObject |
| `Protos.BattleTarget` | 4 | UnitId, HealthDelta, AmountDecimal, extensionObject |
| `Protos.enum BattleTargetingTactic` | 5 | value__, TARGETRANDOM, TARGETRANDOMSWITCH, TARGETRANDOMOVERKILL, TARGETRANDOMWEIGHTEDSWITCH |
| `Protos.BattleTurn` | 3 | BattleTurnOwner, _battleAction, extensionObject |
| `Protos.enum BattleTurnOwner` | 3 | value__, PLAYER, OPPONENT |
| `Protos.enum BattleType` | 12 | value__, PVEBATTLE, PVPBATTLE, WARBATTLE, GUILDRAIDBATTLE, TERRITORYSTRIKEBATTLE, TERRITORYCOVERTBATTLE, TERRITORYWARBATTLE, ARCADERAIDBATTLE, TERRITORYTOURNAMENTBATTLE, CONQUESTBATTLE, COLISEUMBATTLE |
| `Protos.BattleUltimateAbilityStat` | 3 | UnitId, _statCounter, extensionObject |
| `Protos.BattleUltimateAbilityStatCounter` | 3 | Count, ChargePercent, extensionObject |
| `Protos.BattleUnitAbilityState` | 3 | AbilityId, Cooldown, extensionObject |
| `Protos.BattleUnitCategoryStat` | 3 | UnitCategory, UnitCount, extensionObject |
| `Protos.BattleUnitCritStat` | 3 | UnitCategory, CritCount, extensionObject |
| `Protos.BattleUnitDamageStat` | 3 | UnitCategory, CumulativeDamageAmount, extensionObject |
| `Protos.BattleUnitDeathStat` | 3 | UnitCategory, DeathCount, extensionObject |
| `Protos.BattleUnitHealingStat` | 3 | UnitCategory, CumulativeHealingAmount, extensionObject |
| `Protos.BattleUnitKillStat` | 3 | UnitCategory, KillCount, extensionObject |
| `Protos.BattleUnitState` | 18 | UnitId, HealthPercent, _abilityState, ProgressItem, CurrentHealth, ShieldPercent, TurnPercent, MaxHealth, HasFled, MaxShield, IsCommander, IsDeployed, CurrentShield, OverkillItem, OverkillDamage, BattleContextMaxShield, IsUnscathed, extensionObject |
| `Protos.enum BattleUnitStateStat` | 7 | value__, BATTLEUNITSTATUNDEFINED, BATTLEUNITSTATHEALTH, BATTLEUNITSTATHEALTHPERCENT, BATTLEUNITSTATSPEEDFILL, BATTLEUNITSTATSPEEDFILLPERCENT, BATTLEUNITSTATSHIELDPERCENT |
| `Protos.BattleUnitTurnStat` | 3 | UnitId, TurnCount, extensionObject |
| `Protos.enum BindOption` | 4 | value__, BIND, DONTBIND, BINDOPTIONNULL |
| `Protos.Bucket` | 3 | Id, _slot, extensionObject |
| `Protos.BucketCard` | 5 | _bucketItem, TimesToOpen, DescriptionKey, DescriptionValue, extensionObject |
| `Protos.BucketItem` | 12 | DefaultHash1, DefaultHash2, Id, Type, MinQuantity, MaxQuantity, Rarity, StatMod, PreviewPriority, UnitLevel, UnitTier, PrimaryReward |
| `Protos.BucketSlot` | 2 | _item, extensionObject |
| `Protos.BulkClaimDailyRewardRequest` | 2 | _dailyReward, extensionObject |
| `Protos.BulkClaimDailyRewardResponse` | 5 | Delta, _id, Player, LevelUpDelta, extensionObject |
| `Protos.BuyMerchantNodeItemRequest` | 3 | NodeIdentifier, SelectedIndex, extensionObject |
| `Protos.BuyMerchantNodeItemResponse` | 2 | PurchasedResult, extensionObject |
| `Protos.CachedRaidRewards` | 3 | _rankRewardPreview, _immediateRegularRankRewardPreview, extensionObject |
| `Protos.enum CalendarCatchUp` | 4 | value__, DISABLED, MISSEDDAY, ANYDAY |
| `Protos.CalendarCategoryDefinition` | 5 | Id, Unlimited, SortOrder, TextKey, extensionObject |
| `Protos.enum CalendarCompletionBehavior` | 4 | value__, REMAIN, REMOVE, REPEAT |
| `Protos.CalendarExpiry` | 4 | Duration, EndTime, ExpiryRequirement, extensionObject |
| `Protos.enum CalendarMissedConsequence` | 4 | value__, IGNOREMISSED, SKIPMISSED, RESETONMISS |
| `Protos.CalendarVisuals` | 3 | Background, Button, extensionObject |
| `Protos.Campaign` | 12 | Id, NameKey, DescriptionKey, Image, CampaignType, _campaignMap, Progress, UnlockRequirementLocalizationKey, DailyBattleCapKey, EntryCategoryAllowed, GrindEnabled, extensionObject |
| `Protos.CampaignCinematic` | 4 | Trigger, Environment, SubsequentViewingEnvironment, extensionObject |
| `Protos.CampaignElementIdentifier` | 6 | CampaignId, CampaignMapId, CampaignNodeId, CampaignNodeDifficulty, CampaignMissionId, extensionObject |
| `Protos.enum CampaignElementProgress` | 8 | value__, HIDDEN, LOCKED, UNLOCKED, COMPLETE, COMPLETEONESTAR, COMPLETETWOSTAR, COMPLETETHREESTAR |
| `Protos.CampaignMap` | 11 | Id, _campaignNodeDifficultyGroup, Progress, _entryOwnershipRequirement, UnlockRequirementLocalizationKey, IsPlayerSquadSupplied, DailyBattleCapKey, EntryCategoryAllowed, GrindEnabled, GameModeKey, extensionObject |
| `Protos.CampaignMissionEnemyPreview` | 17 | BaseEnemyItem, EnemyLevel, EnemyTier, ThreatLevel, ThumbnailName, PrefabName, DisplayedEnemy, UnitClass, EnemyForceAlignment, EnemyRelicTier, ZetaCount, IsGalacticLegend, IsUltimateUnlocked, OmicronCount, EraLevel, EraThumbnailName, extensionObject |
| `Protos.CampaignNode` | 17 | Id, NameKey, _campaignNodeMission, HomePrefab, ForceAlignment, DominantClass, CombatType, IsCannon, Progress, UnlockRequirementLocalizationKey, DailyBattleCapKey, EntryCategoryAllowed, CooldownKey, GrindEnabled, CooldownConditionalOverrideKey, _firstTimeFinishCost, extensionObject |
| `Protos.enum CampaignNodeDifficulty` | 12 | value__, NOTSET, NORMALDIFF, HARDDIFF, NIGHTMAREDIFF, SPECIALDIFF, IDIFF, IIDIFF, IIIDIFF, IVDIFF, VDIFF, VIDIFF |
| `Protos.CampaignNodeDifficultyGroup` | 8 | CampaignNodeDifficulty, _campaignNode, Progress, UnlockRequirementLocalizationKey, DailyBattleCapKey, EntryCategoryAllowed, GrindEnabled, extensionObject |
| `Protos.CampaignNodeEncounter` | 21 | _encounterCinematics, _environment, _encounterMessage, AudioKey, AllowChanceMechanics, _firstTimeEncounterMessage, EncounterIcon, EncounterDescKey, EncounterIconPosition, RaidProgressColor, MaxDeployedSquadSize, EnvironmentCollectionId, EncounterId, EncounterDurationLimit, ScoreToComplete, DetailDescKey, PointsToComplete, AudioVictoryKey, _displayableEnemyId, _displayableGrantedAbility, extensionObject |
| `Protos.CampaignNodeMission` | 61 | Id, NameKey, DescKey, CombatType, _enemyPreview, _rewardPreview, _campaignNodeEncounter, AudioKey, Progress, _entryCostRequirement, EntryCategoryAllowed, ShortNameKey, GroupNameKey, GroupImage, UnlockRequirementLocalizationKey, CooldownKey, DailyBattleCapKey, _firstCompleteRewardPreview, _enemyUnitPreview, Prefab, Position, _playerAbilityRef, _activateCost, _rankRewardPreview, … (+37) |
| `Protos.CampaignNodePosition` | 3 | X, Y, extensionObject |
| `Protos.CampaignStatusUpdateMessage` | 6 | CampaignElementIdentifier, Progress, SimmableOverrideEnabled, WinCount, LastSuccessfulBattleTime, extensionObject |
| `Protos.CampaignStatusUpdateMessageSet` | 2 | _message, extensionObject |
| `Protos.enum CampaignType` | 13 | value__, STANDARD, ONBOARDING, CAMPAIGNEVENT, GUILDRAID, CAMPAIGNPVELIGHT, CAMPAIGNPVEDARK, CAMPAIGNPVEMODBATTLES, CAMPAIGNPVEHOLOCRON, CAMPAIGNSHIPPVE, TERRITORYBATTLE, ARCADERAIDBATTLES, COLISEUMPVE |
| `Protos.CancelGiftPurchaseRequest` | 4 | TransactionId, StoreItemId, RecipientPlayerId, extensionObject |
| `Protos.CancelGiftPurchaseResponse` | 1 | extensionObject |
| `Protos.CancelJoinGuildRequest` | 1 | extensionObject |
| `Protos.CancelJoinGuildResponse` | 1 | extensionObject |
| `Protos.CancelRaidRequest` | 2 | RaidDefId, extensionObject |
| `Protos.CancelRaidResponse` | 3 | _raidStatus, _raidResult, extensionObject |
| `Protos.CantinaCharacterDialog` | 2 | Texture, extensionObject |
| `Protos.CascadingStoreOffer` | 11 | Id, _storeItemId, ShowRequirement, TitleKey, DescriptionKey, StartTimeSeconds, EndTimeSeconds, HomeIcon, _storeLocation, WebStoreCascadingOffer, extensionObject |
| `Protos.CatchUpCalendarRequest` | 3 | Id, RewardIndex, extensionObject |
| `Protos.CatchUpCalendarResponse` | 4 | Delta, Player, LevelUpDelta, extensionObject |
| `Protos.Category` | 5 | Id, DescKey, Visible, _uiFilter, extensionObject |
| `Protos.enum CategoryMatchType` | 3 | value__, MATCHALL, MATCHANY |
| `Protos.Challenge` | 29 | Id, Type, NameKey, DescKey, ViewRequirement, _reward, PlatformAchievementId, ActionLinkDef, ClaimType, RewardType, RewardRatio, _task, MessageGroup, StyleId, Priority, NextChallengeId, RewardDialogKey, RewardRedirectToHome, RewardDescKey, StarterGuildGraduation, RewardPoints, DetailsKey, HiddenInMainQuests, ClaimRequirement, … (+5) |
| `Protos.enum ChallengeClaimType` | 4 | value__, CHALLENGECLAIMMANUAL, CHALLENGECLAIMAUTOMATIC, CHALLENGECLAIMDAILY |
| `Protos.ChallengeProgress` | 8 | Id, CurrentValue, CompleteValue, Claimed, ExpireTime, EventInstanceId, CampaignElementId, extensionObject |
| `Protos.enum ChallengeRewardType` | 3 | value__, CHALLENGEREWARDFIXED, CHALLENGEREWARDRATIO |
| `Protos.ChallengeStyle` | 9 | Id, ButtonIcon, CharacterIcon, CharacterNameKey, CharacterRewardIcon, CompleteKey, AudioKey, QuestTitleKey, extensionObject |
| `Protos.ChallengeTask` | 6 | Id, NameKey, DescKey, ActionLinkDef, HideToast, extensionObject |
| `Protos.enum ChallengeType` | 10 | value__, DAILYCHALLENGE, LIFETIMECHALLENGE, QUESTCHALLENGE, PLAYERACTIVATEDCHALLENGE, FEATCHALLENGE, EPISODEQUESTBLOCK, LOYALTYMILESTONE, CHARACTERQUESTBLOCK, RETURNINGUSEREXPERIENCEQUEST |
| `Protos.ChallengeUpdate` | 4 | _updated, ChallengeResetTime, _removed, extensionObject |
| `Protos.enum ChanceRollType` | 3 | value__, PERUNIT, ALLTARGETS |
| `Protos.ChannelEvent` | 11 | Id, CorrelationId, ChannelId, EventType, EventSubtype, Timestamp, AuthorId, AuthorName, Message, _data, extensionObject |
| `Protos.ChannelEventData` | 3 | Type, Payload, extensionObject |
| `Protos.enum ChannelEventDataType` | 10 | value__, MESSAGEPARAM, TERRITORYCONFLICTACTIVITY, TERRITORYSTRIKEACTIVITY, TERRITORYRECONACTIVITY, TERRITORYMAPACTIVITY, TERRITORYCOVERTACTIVITY, TERRITORYWARCONFLICTACTIVITY, CHATSOCIALSTATUS, CHATSEASONLEAGUESTATUS |
| `Protos.ChannelEventRequest` | 7 | ChannelId, LastUpdateTimestamp, MinTimestamp, MaxTimestamp, Limit, Offset, extensionObject |
| `Protos.enum ChannelEventType` | 4 | value__, DEFAULT, CHATMESSAGE, ACTIVITYEVENT |
| `Protos.ChannelPermission` | 2 | GroupId, extensionObject |
| `Protos.ChannelSession` | 5 | AuthId, AuthToken, DisplayName, _permission, extensionObject |
| `Protos.CharacterDialog` | 6 | CharacterPrefab, _animations, AudioKey, Layout, UseHologramEffect, extensionObject |
| `Protos.CharacterQuestLine` | 7 | StartTime, EndTime, _characterQuestBlock, UnitId, Image, TitleKey, extensionObject |
| `Protos.enum ChatInboxWhitelistCategory` | 5 | value__, EVERYONE, ALLIES, GUILD, NOBODY |
| `Protos.enum ChatInteraction` | 3 | value__, OPEN, CLOSE |
| `Protos.ChatSeasonLeagueStatus` | 2 | SeasonsLeagueId, extensionObject |
| `Protos.ChatSocialStatus` | 7 | Portrait, Title, GuildLogo, GuildColor, GuildName, PlayerLevel, extensionObject |
| `Protos.ChatTelemetryRequest` | 5 | ChatInteraction, ChatsSent, RoomId, Type, extensionObject |
| `Protos.ChatTelemetryResponse` | 1 | extensionObject |
| `Protos.ChooseDatacronRerollOptionRequest` | 3 | DatacronId, RerollIndex, extensionObject |
| `Protos.ChooseDatacronRerollOptionResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.CinematicTrigger` | 2 | TriggerCondition, extensionObject |
| `Protos.ClaimAllEnergyRewardsRequest` | 1 | extensionObject |
| `Protos.ClaimAllEnergyRewardsResponse` | 2 | Player, extensionObject |
| `Protos.ClaimAppRatingIncentiveRequest` | 1 | extensionObject |
| `Protos.ClaimAppRatingIncentiveResponse` | 2 | Delta, extensionObject |
| `Protos.ClaimChallengeRequest` | 2 | Id, extensionObject |
| `Protos.ClaimChallengeResponse` | 3 | _delta, PlayerUpdate, extensionObject |
| `Protos.ClaimDailyRewardRequest` | 3 | CurrentRewardIndex, Id, extensionObject |
| `Protos.ClaimDailyRewardResponse` | 4 | Delta, Player, LevelUpDelta, extensionObject |
| `Protos.ClaimEnergyRewardRequest` | 2 | EnergyType, extensionObject |
| `Protos.ClaimEnergyRewardResponse` | 2 | Player, extensionObject |
| `Protos.ClaimEpisodeTrackRewardRequest` | 4 | _regularRewardIndex, _premiumRewardIndex, IncludeRepeatable, extensionObject |
| `Protos.ClaimEpisodeTrackRewardResponse` | 2 | Delta, extensionObject |
| `Protos.ClaimImmediateRewardRequest` | 4 | ImmediateRewardIndex, _regularImmediateRewardIndex, _premiumImmediateRewardIndex, extensionObject |
| `Protos.ClaimImmediateRewardResponse` | 2 | Delta, extensionObject |
| `Protos.ClaimMilestonesRequest` | 3 | _eventInstanceId, _eventTier, extensionObject |
| `Protos.ClaimMilestonesResponse` | 2 | _updatedProgress, extensionObject |
| `Protos.ClaimQuestsAndHeadlinesRequest` | 3 | _challengeId, _questBlockId, extensionObject |
| `Protos.ClaimQuestsAndHeadlinesResponse` | 3 | _delta, PlayerUpdate, extensionObject |
| `Protos.ClaimTotalEraLevelRewardRequest` | 2 | RewardRank, extensionObject |
| `Protos.ClaimTotalEraLevelRewardResponse` | 2 | Delta, extensionObject |
| `Protos.ClaimWarChestRequest` | 2 | WarId, extensionObject |
| `Protos.ClaimWarChestResponse` | 4 | _inventoryDelta, PlayerUpdate, WarStatus, extensionObject |
| `Protos.ClaimWindowsStoreOfferRequest` | 4 | OfferId, ReceiptTransactionId, PurchaseContextId, extensionObject |
| `Protos.ClaimWindowsStoreOfferResponse` | 5 | InventoryDelta, AmountSpent, LastSpentTime, Player, extensionObject |
| `Protos.ClearChannelSessionRequest` | 2 | _authId, extensionObject |
| `Protos.ClearChannelSessionResponse` | 1 | extensionObject |
| `Protos.ClearPlayerPortraitRequest` | 1 | extensionObject |
| `Protos.ClearPlayerPortraitResponse` | 1 | extensionObject |
| `Protos.ClearPlayerTitleRequest` | 1 | extensionObject |
| `Protos.ClearPlayerTitleResponse` | 1 | extensionObject |
| `Protos.enum ClientCacheDomain` | 5 | value__, ASSETS, GAMEDATA, PLAYERPREFS, LOCALIZATION |
| `Protos.ClientCacheEntry` | 3 | Domain, Version, extensionObject |
| `Protos.enum ClientCommandArgumentType` | 3 | value__, CONSTANT, PROMPT |
| `Protos.ClientQualityData` | 6 | PerformanceLevel, MemoryLevel, GcActivityLevel, VfxAllocationLevel, _flag, extensionObject |
| `Protos.ClientSegment` | 3 | SegmentId, SegmentHash, extensionObject |
| `Protos.ClientSpecs` | 16 | GlExtensionData, Platform, DeviceModel, OperatingSystem, SystemMemorySize, GraphicsMemorySize, GraphicsDeviceName, GraphicsDeviceVendor, GraphicsDeviceVersion, ProcessorType, ProcessorCount, BundleId, ExternalVersion, InternalVersion, Region, extensionObject |
| `Protos.ClientVersion` | 3 | Min, Max, extensionObject |
| `Protos.ColiseumBossDefinition` | 10 | Identifier, UnitPrefab, EnvironmentPrefab, _bossRewardTable, CantinaPrefab, BossHighlightText, ShortIntroCinematic, MediumIntroCinematic, LongIntroCinematic, extensionObject |
| `Protos.ColiseumBossRewardTableDefinition` | 4 | MissionId, BossProgressionRewardTableId, _bossProgressionRewardPreview, extensionObject |
| `Protos.ColiseumBossSchedule` | 6 | BossIndex, StartTime, ReviewTime, EndTime, EraDayIndex, extensionObject |
| `Protos.ColiseumBossStatus` | 4 | Id, CurrentTier, BestTierScore, extensionObject |
| `Protos.ColiseumDailyRewardRequest` | 1 | extensionObject |
| `Protos.ColiseumDailyRewardResponse` | 4 | Delta, EraStatus, LeaderboardRank, extensionObject |
| `Protos.ColiseumDefinition` | 5 | DailyRankRewardTableId, _bossDefinition, _dailyRankRewardPreview, _bossSchedule, extensionObject |
| `Protos.ColiseumOverviewData` | 2 | ShardLeaderboardRank, extensionObject |
| `Protos.ColiseumStatus` | 3 | _bossStatus, LastDailyRewardClaimTimeMs, extensionObject |
| `Protos.enum ColorType` | 7 | value__, WHITE, RED, ORANGE, YELLOW, GREEN, BLUE |
| `Protos.enum CombatType` | 3 | value__, CHARACTER, SHIP |
| `Protos.CommandMessages` | 3 | TerritoryMapId, _messages, extensionObject |
| `Protos.CommandTerritoryRequest` | 7 | TerritoryMapId, ZoneId, Message, OrderState, ZoneInstance, SendNotification, extensionObject |
| `Protos.CommandTerritoryResponse` | 3 | Message, LastNotificationTime, extensionObject |
| `Protos.ComparisonParamsInt` | 3 | ComparisonType, Value, extensionObject |
| `Protos.enum ComparisonType` | 5 | value__, ATLEAST, LESSTHAN, EQUAL, MORETHAN |
| `Protos.enum CompletionUnitType` | 3 | value__, COAXIUM, POINTS |
| `Protos.enum ConditionType` | 103 | value__, ROUNDCOUNT, ENDOFROUND, STARTOFROUND, STARTOFBATTLE, CHANCE, STARTOFENCOUNTER, CRIT, RECEIVECRIT, PERFORMMOVE, ATTACK, LIMITBREAK, RECEIVEDAMAGE, RECEIVEHEAL, COMBO, RECEIVECOMBO, KILL, REVIVE, ENEMYKILLED, ALLYKILLED, DEATH, ALLYREVIVED, ENEMYREVIVED, ENDOFENCOUNTER, … (+79) |
| `Protos.ConditionalBucket` | 3 | RequirementId, _bucketItem, extensionObject |
| `Protos.ConfigEntry` | 3 | Key, Value, extensionObject |
| `Protos.ConfirmEAAppGiftPurchaseRequest` | 3 | StoreItemId, RecipientPlayerId, extensionObject |
| `Protos.ConfirmEAAppGiftPurchaseResponse` | 1 | extensionObject |
| `Protos.ConfirmStorePurchaseSuccessRequest` | 6 | TransactionId, StoreItemId, RecipientPlayerId, ClientIpAddress, PurchaseContextId, extensionObject |
| `Protos.ConfirmStorePurchaseSuccessResponse` | 1 | extensionObject |
| `Protos.ConquestDefinition` | 10 | Id, NameKey, DescriptionKey, _conquestDifficulty, StaminaStatAllowlist, _consumableDetail, _visibility, _miscEntry, ChaseCharacterTexture, extensionObject |
| `Protos.ConquestDifficulty` | 9 | MinGp, _sector, NameKey, Id, _rankRewardPreview, _immediateRankRewardPreview, RecommendedMinGp, _premiumImmediateRankRewardPreview, extensionObject |
| `Protos.ConquestDynamicDifficultyInfo` | 3 | DifficultyId, _nodeInfo, extensionObject |
| `Protos.ConquestDynamicNodePreview` | 6 | NodeId, _enemyUnitPreview, _grantedAbility, _artifactItem, _merchantItem, extensionObject |
| `Protos.ConquestDynamicPreview` | 2 | _nodePreview, extensionObject |
| `Protos.ConquestEventDetails` | 3 | StartTime, EndTime, extensionObject |
| `Protos.ConquestMission` | 8 | Id, BattleDetails, _entryCostRequirement, _rewardPreview, _conditionalRewardsPreview, _instanceFirstCompleteRewardPreview, RecommendationKey, extensionObject |
| `Protos.ConquestNode` | 10 | Id, Type, ConquestMissionId, NameKey, _enemyUnitPreview, ConquestCompletionNode, UnlockRequirementId, Style, FinishSector, extensionObject |
| `Protos.ConquestNodeSelection` | 3 | NodeId, SelectedItem, extensionObject |
| `Protos.ConquestNodeStatus` | 4 | Progress, SectorId, NodeId, extensionObject |
| `Protos.enum ConquestNodeStyle` | 4 | value__, DEFAULTSTYLE, LEGEND, DATACRONSTYLE |
| `Protos.enum ConquestNodeType` | 7 | value__, COMBATNODE, ARTIFACTNODE, MERCHANTNODE, STARTNODE, BOSSNODE, COMBATNOPOINTSNODE |
| `Protos.ConquestPurchasePassMiscTextEntry` | 4 | SpriteName, TextId, Type, extensionObject |
| `Protos.enum ConquestPurchasePassMiscTextEntryType` | 4 | value__, PASSWITHEXTRAS, PASSONLY, ALLPASSVARIANTS |
| `Protos.ConquestSector` | 8 | Id, _node, PrefabName, ArtTile, TitleKey, EnvironmentTitleKey, BackgroundPrefabName, extensionObject |
| `Protos.ConquestStatus` | 19 | ConquestEventIdentifier, DifficultyType, ConquestDefinitionIdentifier, _equippedArtifact, _unequippedArtifact, _nodeStatus, _obtainedConsumable, _activeConsumable, _unitStamina, DynamicPreviewCacheLastInvalidTime, _nodeSelection, _claimedImmediateRewards, BattlePassActive, _claimedPremiumImmediateRewards, StaminaRegenTimeReduction, EnergyRegenTimeReduction, ConquestFreeArtifactSwapActivated, BattlePassId, extensionObject |
| `Protos.ConquestStatusRequest` | 1 | extensionObject |
| `Protos.ConquestStatusResponse` | 3 | Status, Energy, extensionObject |
| `Protos.ConquestUnitStamina` | 4 | UnitId, RemainingStamina, LastRefreshTime, extensionObject |
| `Protos.ConquestVisibility` | 3 | Type, Distance, extensionObject |
| `Protos.Consumable` | 6 | Id, DefinitionId, UnitAppliedId, BattlesRemaining, ExpireTimeInSeconds, extensionObject |
| `Protos.ConsumableDefinition` | 15 | Id, TierId, IconKey, NameKey, DescriptionKey, TotalBattleUse, _grantedAbility, StaminaRecovery, _targetedStat, ConsumableTypeId, EffectIconKey, ActiveTimeInSeconds, StaminaRegenTimeReduction, EnergyRegenTimeReduction, extensionObject |
| `Protos.ConsumableDetails` | 3 | TypeId, MaxCount, extensionObject |
| `Protos.ConsumableTierDefinition` | 6 | Id, Rarity, NameKey, IconKey, EffectIconFrameKey, extensionObject |
| `Protos.ConsumableType` | 5 | TypeId, NameKey, DescriptionKey, IconKey, extensionObject |
| `Protos.enum ContentEncoding` | 3 | value__, DEFAULTCONTENTENCODING, GZIPCONTENTENCODING |
| `Protos.ContentSegment` | 3 | Name, _files, extensionObject |
| `Protos.ContentSegmentFile` | 6 | Id, Filters, Path, GroupId, Size, extensionObject |
| `Protos.ContextualMessage` | 21 | Id, Type, TitleKey, TextKey, PointAt, _appearEvent, _dismissEvent, _hideEvent, ActionLink, NextMessageId, ClientRequirement, CharacterDialog, NextMessageIdDecline, CanDismissWhileHidden, FtueSubStep, CantinaCharacterDialog, ActionLinkDef, StorePromoDialog, ShowExitButton, MessageContextData, extensionObject |
| `Protos.ContextualMessageGroup` | 9 | GroupId, StartMessage, _message, Priority, CanDismissWhileHidden, StartTime, EndTime, IsFtue, extensionObject |
| `Protos.ContextualMessageGroupSet` | 3 | _addedSet, _removedSet, extensionObject |
| `Protos.enum ContextualMessageType` | 11 | value__, POINTATVIEWELEMENT, CHARACTERDIALOG, MODALDIALOGONEBUTTON, MODALDIALOGTWOBUTTON, CANTINACHARACTERDIALOG, UNITUNLOCK, STOREPROMODIALOG, CHALLENGEQUESTREWARDS, RETURNINGUSEREXPERIENCEDIALOG, REWARDPREVIEWDIALOG |
| `Protos.ContextualStoreItem` | 3 | Id, _storeItemId, extensionObject |
| `Protos.ConversionItem` | 4 | Id, Type, PointValue, extensionObject |
| `Protos.ConversionItemPrioritized` | 3 | Item, Priority, extensionObject |
| `Protos.ConversionItemQuantified` | 3 | Item, Quantity, extensionObject |
| `Protos.ConversionSet` | 4 | Output, _consumable, Surplus, extensionObject |
| `Protos.Cooldown` | 3 | Id, RefreshCost, extensionObject |
| `Protos.CooldownStatus` | 3 | Id, CooldownTime, extensionObject |
| `Protos.CooldownStatusUpdate` | 2 | _status, extensionObject |
| `Protos.CraftConversionItemRequest` | 3 | Output, _consumable, extensionObject |
| `Protos.CraftItemRequest` | 2 | _recipeId, extensionObject |
| `Protos.CraftItemResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.CraftSkillRequest` | 4 | UnitId, SkillId, SkillTier, extensionObject |
| `Protos.CraftSkillResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.CraftingMaterial` | 4 | Id, Quantity, BonusQuantity, extensionObject |
| `Protos.CraftingMaterialDef` | 19 | Id, NameKey, DescKey, IconKey, SellValue, _lookupMission, XpValue, Type, Rarity, TrainingCost, TrainingCostMaxLevel, UnitDefReference, Tier, ObtainableTime, _raidLookup, _actionLinkLookup, RecipeId, _raidImmediateLookup, extensionObject |
| `Protos.CreateChannelSessionRequest` | 4 | AuthId, DisplayName, _permission, extensionObject |
| `Protos.CreateChannelSessionResponse` | 2 | AuthToken, extensionObject |
| `Protos.CreateExchangeDonationRequestRequest` | 2 | RequestedItem, extensionObject |
| `Protos.CreateExchangeDonationRequestResponse` | 2 | Status, extensionObject |
| `Protos.CreateGameChannelSessionRequest` | 1 | extensionObject |
| `Protos.CreateGameChannelSessionResponse` | 3 | AuthId, AuthToken, extensionObject |
| `Protos.CreateGuildRequest` | 2 | Profile, extensionObject |
| `Protos.CreateGuildResponse` | 4 | Guild, InventoryDelta, PlayerDelta, extensionObject |
| `Protos.CreateRoomRequest` | 4 | Type, Name, _memberPlayerId, extensionObject |
| `Protos.CreateRoomResponse` | 3 | Room, _badInvite, extensionObject |
| `Protos.CrewMember` | 5 | UnitId, Slot, _skillReference, SkilllessCrewAbilityId, extensionObject |
| `Protos.CurrencyItem` | 4 | Currency, Quantity, BonusQuantity, extensionObject |
| `Protos.enum CurrencyType` | 48 | value__, GRIND, PREMIUM, REALMONEY, SOCIAL, IGCEVENTKEYCARD, XPEVENTKEYCARD, PVPCURRENCY, FORCEPOINT, PVPSHIPCURRENCY, FREE, WARSHIPCURRENCY, GRINDTICKET, SHARDCURRENCY, GUILDCURRENCY, GUILDBANKCURRENCY, SHIPGRIND, GUILDRAIDCURRENCY01, GUILDRAIDCURRENCY02, GUILDRAIDCURRENCY03, GUILDRAIDCURRENCY04, GUILDRAIDCURRENCY05, GUILDRAIDCURRENCY06, GUILDRAIDCURRENCY07, … (+24) |
| `Protos.CurrencyWithAmount` | 3 | Currency, Amount, extensionObject |
| `Protos.DailyActionCap` | 5 | Id, MaxActions, _purchaseCost, UnlimitedPurchases, extensionObject |
| `Protos.DailyActionCapStatus` | 6 | Id, ActionsUsed, PurchaseCount, StatusExpireTime, Expires, extensionObject |
| `Protos.DailyActionCapStatusUpdate` | 2 | _status, extensionObject |
| `Protos.DailyLogin` | 13 | DailyLoginTitle, DailyLoginMessage, StartTime, ExpireTime, CurrentRewardIndex, LastClaimed, _dailyReward, Claimed, Id, Category, ExpiryRequirement, DailyClaimedBitmask, extensionObject |
| `Protos.DailyLoginRewardDefinition` | 18 | Id, Title, Message, DefaultBucketItem, _reward, Monthly, MissedConsequence, CompletionBehavior, Icon, Category, DelayFirstReward, CatchUp, Expiry, VisualOverride, CatchupCost, UseServerResetTime, ResetTimeOffset, extensionObject |
| `Protos.DailyLoginUpdate` | 8 | Id, StartTime, EndTime, CurrentRewardIndex, LastClaimed, Claimed, DailyClaimedBitmask, extensionObject |
| `Protos.Datacron` | 11 | Id, SetId, TemplateId, _tag, _affix, Locked, RerollIndex, _rerollOption, RerollCount, Focused, extensionObject |
| `Protos.DatacronAffix` | 9 | TargetRule, AbilityId, StatType, StatValue, _tag, RequiredUnitTier, RequiredRelicTier, ScopeIcon, extensionObject |
| `Protos.DatacronAffixBattleTelemetry` | 3 | AffixApplied, AffixTargeted, extensionObject |
| `Protos.DatacronAffixTemplate` | 10 | AbilityId, TargetRule, StatType, StatValueMin, StatValueMax, MinTier, MaxTier, _tag, ScopeIcon, extensionObject |
| `Protos.DatacronAffixTemplateSet` | 3 | Id, _affix, extensionObject |
| `Protos.DatacronBattleStat` | 7 | Id, SetId, TemplateId, _tag, _affix, HighestTierApplied, extensionObject |
| `Protos.DatacronHelpEntry` | 3 | Title, Body, extensionObject |
| `Protos.enum DatacronScopeIdentifier` | 5 | value__, STATSCOPE, ALIGNMENTSCOPE, FACTIONSCOPE, CHARACTERSCOPE |
| `Protos.DatacronSet` | 9 | Id, DisplayName, ExpirationTimeMs, Icon, _tier, DetailPrefab, _setMaterial, _focusedTier, extensionObject |
| `Protos.DatacronSetExpirationUpdate` | 4 | FirstExpirationSetId, LastExpirationSetId, InventoryDelta, extensionObject |
| `Protos.DatacronSetMaterial` | 4 | MaterialId, QuantityPerDustGrant, DustGrantRecipeId, extensionObject |
| `Protos.DatacronSetTier` | 6 | Id, UpgradeCostRecipeId, DustGrantRecipeId, _rerollCostRecipeId, ScopeIdentifier, extensionObject |
| `Protos.DatacronTemplate` | 12 | Id, SetId, InitialTiers, ReferenceTemplateId, _fixedTag, _tier, MaxRerolls, AllowReroll, Focused, FocusedIcon, FocusedPrefab, extensionObject |
| `Protos.DatacronTemplateTier` | 8 | Id, _affixTemplateSetId, _initialAffixTemplateSetIds, RequiredUnitTier, RequiredRelicTier, OverrideUpgradeCostRecipeId, OverrideScopeIdentifier, extensionObject |
| `Protos.enum DefenseAction` | 5 | value__, NOACTION, NEW, MODIFIED, DELETED |
| `Protos.DefenseUnit` | 4 | UnitBaseId, UnitType, CellIndex, extensionObject |
| `Protos.DefensesZoneCapacityUpdate` | 2 | _territoryDefenseUpdate, extensionObject |
| `Protos.DeleteModFilterRequest` | 2 | Id, extensionObject |
| `Protos.DeleteModFilterResponse` | 1 | extensionObject |
| `Protos.DeleteModLoadoutRequest` | 3 | TabId, LoadoutId, extensionObject |
| `Protos.DeleteModLoadoutResponse` | 1 | extensionObject |
| `Protos.DeleteModLoadoutTabRequest` | 2 | TabId, extensionObject |
| `Protos.DeleteModLoadoutTabResponse` | 1 | extensionObject |
| `Protos.DeleteRoomRequest` | 2 | RoomId, extensionObject |
| `Protos.DeleteRoomResponse` | 1 | extensionObject |
| `Protos.DeleteSquadPresetTabRequest` | 3 | CombatType, TabId, extensionObject |
| `Protos.DeleteSquadPresetTabResponse` | 1 | extensionObject |
| `Protos.DemoteMemberRequest` | 2 | PlayerId, extensionObject |
| `Protos.DemoteMemberResponse` | 2 | Member, extensionObject |
| `Protos.DeployUnitsRequest` | 7 | TerritoryMapId, ZoneId, _deploymentTarget, EventType, DefensiveSquad, KnownCommandState, extensionObject |
| `Protos.DeployUnitsResponse` | 4 | _territoryBattleStatus, _territoryWarStatus, _territoryTournamentEventStatus, extensionObject |
| `Protos.DeployUnitsTarget` | 5 | UnitId, SquadId, PlatoonId, DefensiveSquad, extensionObject |
| `Protos.DevicePreferences` | 5 | PushnotesAllowed, MusicSetting, SfxSetting, NetworkConnection, extensionObject |
| `Protos.DisambiguateGiftPurchaseRequest` | 3 | StoreItemId, RecipientPlayerId, extensionObject |
| `Protos.DisambiguateGiftPurchaseResponse` | 3 | _purchasedResult, Player, extensionObject |
| `Protos.DisbandGuildRequest` | 1 | extensionObject |
| `Protos.DisbandGuildResponse` | 1 | extensionObject |
| `Protos.DisconnectEAAccountRequest` | 1 | extensionObject |
| `Protos.DisconnectEAAccountResponse` | 1 | extensionObject |
| `Protos.DismantleLightspeedTokenRequest` | 2 | LightspeedTokenId, extensionObject |
| `Protos.DismantleLightspeedTokenResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.DismissContextualMessageRequest` | 2 | Id, extensionObject |
| `Protos.DismissContextualMessageResponse` | 1 | extensionObject |
| `Protos.DismissItemRequest` | 2 | ItemId, extensionObject |
| `Protos.DismissItemResponse` | 1 | extensionObject |
| `Protos.DonateToExchangeDonationRequestRequest` | 3 | RequestId, Quantity, extensionObject |
| `Protos.DonateToExchangeDonationRequestResponse` | 2 | Status, extensionObject |
| `Protos.DropRate` | 4 | Rate, Quantity, SlotItemIndex, extensionObject |
| `Protos.DustDatacronRequest` | 2 | _idList, extensionObject |
| `Protos.DustDatacronResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.DynamicDescription` | 13 | DescriptionType, UnitTexture, UnitNameKey, AbilityNameKey, AbilityDescriptionKey, AbilityTexture, HeaderKey, TextKey, MarginLeft, _textColumn, TextAlignment, BackgroundOverrideSprite, extensionObject |
| `Protos.enum DynamicDescriptionTextAlignmentType` | 4 | value__, TEXTALIGNMENTLEFT, TEXTALIGNMENTRIGHT, TEXTALIGNMENTCENTER |
| `Protos.DynamicDescriptionTextColumn` | 5 | HeaderKey, BodyKey, BodyAlignment, Tag, extensionObject |
| `Protos.enum DynamicDescriptionType` | 7 | value__, LAYOUTTEXT, LAYOUTUNIT, LAYOUTABILITY, LAYOUTHEADER, LAYOUTTEXTCOLUMN, LAYOUTHORIZONTALBREAK |
| `Protos.DynamicMessage` | 4 | Type, Data, MessageId, extensionObject |
| `Protos.enum DynamicMessageType` | 20 | value__, CONTEXTUALMESSAGE, CAMPAIGNSTATUSUPDATE, STOREUPDATE, INBOXUPDATE, GAMEEVENTUPDATE, CHALLENGEUPDATE, SUSPENSIONDETAILS, DAILYACTIONCAPSTATUS, COOLDOWNSTATUS, PLAYERUPDATE, SEASONSTATUSUPDATE, CONQUESTPREVIEWINFO, PLAYERRATING, TERRITORYDEFENSESUPDATE, DATACRONSETEXPIRED, EPISODESTATUSUPDATE, INVENTORYUPDATE, ERASTATUSUPDATE, OVERVIEWDATA |
| `Protos.EADPParameters` | 27 | EadpBaseUrl, EaMobileClientId, EadpBaseUrlClient, EadpAllowedCryptKey, EadpClientId, EadpWindowsEncryptSeed, EadpWindowsContentId, EadpWindowsMultiplayerId, EadpWindowsTitle, EadpWindowsCommerceGroupName, EadpClientSecret, EadpEnv, EadpWindowsAchievementCode, EadpProductId, EadpEnsGameId, EadpSellId, EadpEnsApiKey, EadpEnsApiSecret, EadpContentId, EadpMultiplayerId, EadpEncryptSeed, EadpTitle, EadpCommerceGroupName, EadpUmClientId, … (+3) |
| `Protos.enum EconomyEventSubLocation` | 5 | value__, NOLOCATION, FIND, JOURNEYGUIDE, ONGLASS |
| `Protos.Effect` | 49 | Id, Type, _param, Chance, _target, _descriptiveTag, _effectedTag, _triggerCondition, _effectDefinition, _contextIndex, _expirationCondition, ApplyType, DamageType, PersistentIcon, TriggeredVfx, TriggeredLocKey, MaxBonusMoves, _effectReference, MultiplierAmountDecimal, ContextMultiplierDecimal, AdditiveAmountDecimal, ResultVarianceDecimal, PersistentLocKey, StackingLineOverride, … (+25) |
| `Protos.enum EffectApplyType` | 4 | value__, UNAVOIDABLE, PHYSICAL, MENTAL |
| `Protos.enum EffectDamageType` | 4 | value__, UNMITIGABLE, ATTACKDAMAGE, ABILITYPOWER |
| `Protos.EffectReference` | 4 | Id, _contextIndex, MaxBonusMove, extensionObject |
| `Protos.EffectTag` | 4 | DefaultHash1, DefaultHash2, Tag, Exclude |
| `Protos.EffectTagCriteria` | 3 | Tag, Exclude, extensionObject |
| `Protos.EffectTarget` | 15 | UnitSelect, BattleSide, _unitClass, _forceAlignment, Category, HealthState, _statValue, _activeEffectTagCriteria, BattleDeploymentState, Id, ExcludeSelf, ExcludeSelectedTarget, RequiredCategory, _requiredActiveEffectTagCriteria, extensionObject |
| `Protos.enum EffectTargetBattleSide` | 6 | value__, BOTHSIDES, ALLYSIDE, ENEMYSIDE, PLAYERSIDE, OPPONENTSIDE |
| `Protos.EffectTargetCategory` | 3 | Exclude, CategoryId, extensionObject |
| `Protos.EffectTargetCategoryCriteria` | 4 | Exclude, _categoryId, _category, extensionObject |
| `Protos.enum EffectTargetUnitSelect` | 12 | value__, SELECTALL, SELECTEDTARGET, SELECTRANDOM, SELECTSELF, SELECTPREVIOUS, SELECTLASTREINFORCEMENT, SELECTLOWESTSURVIVABILITY, SELECTHIGHESTSURVIVABILITY, SELECTLEADER, SELECTSUPPORT, SELECTLASTSUMMON |
| `Protos.enum EffectType` | 47 | value__, DAMAGE, RECOVER, PARALYSIS, CURSE, DISPELL, IMMUNITY, BONUSMOVE, MODIFYSTAT, APPLYEFFECT, DISTRIBUTERECOVERY, QUERYCOUNT, QUERYSTAT, REVIVEUNIT, SCRIPTCOMPONENT, FLEE, DESTROY, REINFORCE, GRANTSHIELD, GRANTABILITY, HEALTHLIMIT, BLACKBOARDMODIFY, BLACKBOARDQUERY, STATLIMIT, … (+23) |
| `Protos.Energy` | 7 | Type, Timestamp, MaxEnergy, RegenRate, RegenCost, Overflow, extensionObject |
| `Protos.EnergyBonus` | 3 | Type, Amount, extensionObject |
| `Protos.EnergyBonusTime` | 3 | EnergyType, LastBonusEnergyTime, extensionObject |
| `Protos.EnergyRewardDefinition` | 8 | EnergyType, _instance, ClaimableTitleKey, ClaimableDescKey, UpcomingTitleKey, UpcomingDescKey, ShowRequirement, extensionObject |
| `Protos.EnergyRewardInstance` | 5 | Type, Start, Duration, Amount, extensionObject |
| `Protos.enum EnergyRewardInstanceType` | 2 | value__, HOUROFDAY |
| `Protos.enum EnergyType` | 8 | value__, PVE, PVP, SHIPPVE, SHIPPVP, CANTINAPVE, MOD, CONQUEST |
| `Protos.enum EnrollmentStatus` | 4 | value__, ENROLLMENTOPEN, ENROLLMENTINVITE, ENROLLMENTCLOSED |
| `Protos.EntryCategoryAllowed` | 25 | _categoryId, MaximumAllowedUnitQuantity, MatchType, MinimumRequiredUnitQuantity, MinimumUnitRarity, MinimumOwnedUnitQuantity, MinimumUnitLevel, MinimumUnitTier, MaximumReinforcement, _commanderCategoryId, MinimumReinforcement, MinimumGalacticPower, _mandatoryRosterUnit, _excludeCategoryId, MinimumRelicTier, MinimumModRarity, MinimumAbilityLevelAvg, MinimumAbilityLevelAll, LegendLimit, UnitGuideMinimumRequiredUnitQuantity, BigUnitLimit, MandatoryDatacron, EraExclusive, MinimumEraLevel, … (+1) |
| `Protos.Environment` | 4 | Prefab, Layout, LayoutPrefab, extensionObject |
| `Protos.EnvironmentCollection` | 3 | Id, _battleEnvironment, extensionObject |
| `Protos.EpisodeDefinition` | 18 | Id, StartTime, EndTime, _regularImmediateRankRewardPreview, _premiumImmediateRankRewardPreview, RepeatableMilestone, Title, _episodeQuestBlock, _characterQuestLine, ObjectivesViewBackground, PurchasePassPopupBackground, EpisodePassIcon, PurchasePassPopupCenterImageBack, PurchasePassPopupCenterImageFront, EpisodePassIconFree, EpisodePassIconPremium, _introQuestBlock, extensionObject |
| `Protos.enum EpisodeQuestBlockType` | 3 | value__, EPISODE, INTRO |
| `Protos.EpisodeStatus` | 10 | Id, Points, _claimedRegularImmediateRewards, EpisodePassActive, EpisodePassPlusPurchased, _claimedPremiumImmediateRewards, RepeatableMilestoneClaims, _claimedQuestBlockHeadline, EpisodeQuestBlockType, extensionObject |
| `Protos.EpisodeStatusRequest` | 1 | extensionObject |
| `Protos.EpisodeStatusResponse` | 2 | EpisodeStatus, extensionObject |
| `Protos.Equipment` | 4 | Id, Quantity, BonusQuantity, extensionObject |
| `Protos.EquipmentDef` | 18 | Id, NameKey, IconKey, RequiredLevel, EquipmentStat, RecipeId, Tier, SellValue, _lookupMission, Mark, ObtainableTime, _raidLookup, Type, _actionLinkLookup, RequiredRarity, FindFlowDisabled, _raidImmediateLookup, extensionObject |
| `Protos.EquipmentSlot` | 3 | EquipmentId, Slot, extensionObject |
| `Protos.enum EquipmentType` | 4 | value__, EQUIPMENTEQUIPPABLE, EQUIPMENTUNRELEASED, EQUIPMENTMATERIAL |
| `Protos.EraDefinition` | 24 | Id, NameKey, Icon, _episode, EraShardCount, Coliseum, _eraUnitId, ExpectedEraUnitCount, StartTimeMs, EndTimeMs, _eraLevel, _totalEraLevelsRewardPreview, EraStartJourneyUnitTexture, EraStartMarqueeTexture, JourneyUnit, EraStartMarqueeTextureBackground, EraFrameTable, EraSummaryTexture, EraSummaryTextureBackground, _loanedUnit, StartingLoanedUnitEraLevel, MaxLoanedUnitEraLevel, MissingJourneyUnit, extensionObject |
| `Protos.EraLevelDefinition` | 10 | Level, UpgradeRecipeId, RequiredRarity, MajorMilestone, CurrentUnitLevel, CurrentGearTier, CurrentGearPieces, CurrentRelicTier, CurrentAbilityLevel, extensionObject |
| `Protos.EraStatus` | 6 | Id, ColiseumStatus, _eraUnitStatus, TotalEraLevelRewardsClaimed, LoanedUnitEraLevel, extensionObject |
| `Protos.EraSummary` | 5 | Id, _eraUnit, TotalEraLevel, TotalGalacticPower, extensionObject |
| `Protos.EraUnitStatus` | 3 | UnitBaseId, EraLevel, extensionObject |
| `Protos.enum EvaluationType` | 3 | value__, AND, OR |
| `Protos.EventBonus` | 6 | Id, NameKey, DescKey, IconKey, ScoreModifier, extensionObject |
| `Protos.EventCampaignElements` | 2 | _node, extensionObject |
| `Protos.EventDetails` | 3 | StartTime, EndTime, extensionObject |
| `Protos.EventPlayer` | 5 | Id, Name, Level, AmountSpent, extensionObject |
| `Protos.EventRulesStatus` | 4 | _eventModifierOld, _eventBonusId, _eventModifier, extensionObject |
| `Protos.EventSampling` | 4 | EventId, SpenderValue, NonSpenderValue, extensionObject |
| `Protos.ExchangeDonationRequest` | 13 | RequestId, PlayerId, ExpireTimestamp, RequestedItem, RequestorReceivedQuantity, RequestorRequestedQuantity, PlayerDonationQuantity, PlayerMaxDonationQuantity, PlayerAvailableQuantity, _reward, CreationTimestamp, PlayerName, extensionObject |
| `Protos.ExchangeDonationStatus` | 6 | _request, NextDonationRequestTimestamp, CurrentActiveDonationRequestCount, MaxActiveDonationRequestCount, InventoryDelta, extensionObject |
| `Protos.ExecuteCommandRequest` | 3 | Command, Arguments, extensionObject |
| `Protos.ExecuteCommandResponse` | 16 | Message, SuggestRestart, Player, InventoryDelta, UpdatedRoster, AllyList, Guild, TerritoryBattleStatus, TerritoryWarStatus, TerritoryTournamentEventStatus, ConquestStatus, _configOverride, EpisodeStatus, _storeItemPurchaseResetId, EraStatus, extensionObject |
| `Protos.FailedStoreTransactionEvent` | 9 | PlayerId, TransactionId, ItemId, StorePlatformId, StoreInitiatedTime, StoreReceiptTime, Receipt, FailedReasonCode, extensionObject |
| `Protos.Feat` | 2 | ChallengeId, extensionObject |
| `Protos.FeaturedCommand` | 7 | CommandName, DisplayTextKey, DisplayTextFallback, _argument, _prompt, _command, extensionObject |
| `Protos.FeaturedCommandArgument` | 5 | Value, PromptTextKey, PromptTextFallback, Type, extensionObject |
| `Protos.FeaturedCommandClientCommand` | 2 | _argument, extensionObject |
| `Protos.FeaturedCommandClientCommandArgument` | 3 | Type, Value, extensionObject |
| `Protos.FeaturedCommandPrompt` | 6 | Id, MessageKey, MessageFallback, Type, DefaultValue, extensionObject |
| `Protos.Filter` | 9 | MinPlayerLevel, MaxPlayerLevel, MinPlayerSpend, MaxPlayerSpend, MinPveMapUnlocked, MinPveMapNodeUnlocked, MaxPveMapUnlocked, MaxPveMapNodeUnlocked, extensionObject |
| `Protos.enum ForceAlignment` | 4 | value__, NEUTRAL, LIGHT, DARK |
| `Protos.ForcedPlayerSquad` | 3 | Squad, _squadUnit, extensionObject |
| `Protos.ForgetMeRequest` | 1 | extensionObject |
| `Protos.ForgetMeResponse` | 1 | extensionObject |
| `Protos.enum FtueStepAction` | 3 | value__, DISMISSED, SHOWN |
| `Protos.GalacticBundle` | 8 | Id, TargetPlayerLevel, _unitUpgrade, _tabs, _inventoryItem, Info, PrimaryGalacticBundle, extensionObject |
| `Protos.GalacticBundleMainElement` | 7 | _page, ElementType, Image, Title, Description, InventoryType, extensionObject |
| `Protos.enum GalacticBundleMainElementInventoryType` | 7 | value__, GALACTICBUNDLEMAINELEMENTINVENTORYTYPENONE, GALACTICBUNDLEMAINELEMENTINVENTORYTYPECHARACTERS, GALACTICBUNDLEMAINELEMENTINVENTORYTYPESHIPS, GALACTICBUNDLEMAINELEMENTINVENTORYTYPEUNITS, GALACTICBUNDLEMAINELEMENTINVENTORYTYPEITEMS, GALACTICBUNDLEMAINELEMENTINVENTORYTYPEALL |
| `Protos.GalacticBundleMainElementPage` | 5 | Image, Title, _description, _sprite, extensionObject |
| `Protos.enum GalacticBundleMainElementType` | 6 | value__, GALACTICBUNDLEMAINELEMENTTYPEPAGINATION, GALACTICBUNDLEMAINELEMENTTYPEINVENTORY, GALACTICBUNDLEMAINELEMENTTYPESPLIT, GALACTICBUNDLEMAINELEMENTTYPEHIGHTLIGHTS, GALACTICBUNDLEMAINELEMENTTYPEACCESS |
| `Protos.GalacticBundleTab` | 4 | MainElement, Image, Text, extensionObject |
| `Protos.GameCenterIdentityVerificationData` | 7 | PublicKeyUrl, Signature, Salt, Timestamp, ValidationPlayerId, BundleId, extensionObject |
| `Protos.GameDataRequest` | 6 | Version, DevicePlatform, IncludePveUnits, RequestSegment, Items, extensionObject |
| `Protos.GameDataResponse` | 83 | _effect, _battleEnvironments, _xpTable, _table, _material, _category, _equipment, _helpEntry, _targetingSet, _eventSampling, _skill, _requirement, _powerUpBundle, GuildBanner, _unlockAnnouncementDefinition, _battleTargetingRule, _environmentCollection, _persistentVfx, SocialStatus, _playerTitle, _playerPortrait, TimeZoneChangeConfig, _effectIconPriority, _ability, … (+59) |
| `Protos.enum GameDataSegment` | 6 | value__, GAMEDATASEGMENTALL, GAMEDATASEGMENT1000, GAMEDATASEGMENT2000, GAMEDATASEGMENT3000, GAMEDATASEGMENT4000 |
| `Protos.GameEvent` | 42 | Id, Priority, NameKey, SummaryKey, DescKey, Image, Type, _instance, Status, CampaignElementIdentifier, SquadType, PreviewKey, ReviewKey, UiLocation, Icon, DefensiveSquadUsageCostEnabled, _encounterMessage, _environment, DescKeyUnitCriteriaMet, DefensiveSquadType, CombatType, ImageEffect, UnlockRequirementLocalizationKey, TerritoryMapId, … (+18) |
| `Protos.enum GameEventDetailsType` | 4 | value__, EVENTDEFAULT, EVENTMODULAR, EVENTMARQUEE |
| `Protos.GameEventInstance` | 21 | Id, StartTime, EndTime, DisplayStartTime, DisplayEndTime, TimeLimited, CampaignElementIdentifier, _rewardPreview, _rankRewardPreview, EntryCategoryAllowed, DefensiveSquadSize, UnitDailyUsageCapKey, RosterRefreshTime, Joined, SummaryKey, RewardTime, DefensiveSquadSaveTimeLimit, SystemModifier, SupportsPushNotifications, _linkedEventId, extensionObject |
| `Protos.enum GameEventStatus` | 3 | value__, EVENTLOCKED, EVENTUNLOCKED |
| `Protos.enum GameEventType` | 14 | value__, SCHEDULED, KEYCARD, PROGRESSION, MODS, PVPEVENT, ECONOMYEVENT, TERRITORYWAREVENT, TERRITORYBATTLEEVENT, TERRITORYTOURNAMENTEVENT, ARCADERAID, SEASON, LIFECYCLED, CONQUESTEVENT |
| `Protos.enum GameEventUILocation` | 8 | value__, GAMEEVENTUILOCATIONTRIALS, GAMEEVENTUILOCATIONEVENTS, GAMEEVENTUILOCATIONMODS, GAMEEVENTUILOCATIONSHIPTRIALS, GAMEEVENTUILOCATIONNONE, GAMEEVENTUILOCATIONSEASONHUB, GAMEEVENTUILOCATIONCONQUEST |
| `Protos.GameEventUpdateMessageSet` | 2 | _updatedEvent, extensionObject |
| `Protos.enum GameFeature` | 2 | value__, PLAYERRENAME |
| `Protos.enum GcActivityLevel` | 4 | value__, GCACTIVITYLOW, GCACTIVITYMEDIUM, GCACTIVITYHIGH |
| `Protos.GetBatchNucleusPlayerPreviewRequest` | 2 | _nucleusId, extensionObject |
| `Protos.GetBatchNucleusPlayerPreviewResponse` | 2 | _playerPreviews, extensionObject |
| `Protos.GetChannelEventsRequest` | 5 | EventCount, LastUpdateTimestamp, LastUpdateInterval, _channelEventRequest, extensionObject |
| `Protos.GetChannelEventsResponse` | 4 | _event, UpdateTimestamp, UpdateInterval, extensionObject |
| `Protos.GetConquestDynamicPreviewRequest` | 3 | SectorId, _nodeRequest, extensionObject |
| `Protos.GetConquestDynamicPreviewResponse` | 1 | extensionObject |
| `Protos.GetConsolidatedGuildRequest` | 1 | extensionObject |
| `Protos.GetConsolidatedGuildResponse` | 8 | Profile, _member, _inviteStatus, _recentRaidResult, _recentTerritoryWarResult, _lastRaidPointsSummary, _recentTerritoryBattleResult, extensionObject |
| `Protos.GetContextPlayerRequest` | 1 | extensionObject |
| `Protos.GetContextPlayerResponse` | 2 | Player, extensionObject |
| `Protos.GetEventStatusRequest` | 2 | _cachedUnitSnapshotMapId, extensionObject |
| `Protos.GetEventStatusResponse` | 4 | _territoryBattleStatus, _territoryBattleResult, _territoryWarStatus, extensionObject |
| `Protos.GetEventsRequest` | 1 | extensionObject |
| `Protos.GetEventsResponse` | 2 | _gameEvent, extensionObject |
| `Protos.GetExchangeDonationRequestsRequest` | 1 | extensionObject |
| `Protos.GetExchangeDonationRequestsResponse` | 2 | Status, extensionObject |
| `Protos.GetExchangeStoreItemByCurrencyRequest` | 2 | _currency, extensionObject |
| `Protos.GetExchangeStoreItemByCurrencyResponse` | 2 | StoreItem, extensionObject |
| `Protos.GetGiftEligibilityRequest` | 3 | StoreItemId, _playerId, extensionObject |
| `Protos.GetGiftEligibilityResponse` | 2 | _playerId, extensionObject |
| `Protos.GetGuildChallengesRequest` | 1 | extensionObject |
| `Protos.GetGuildChallengesResponse` | 4 | _challenge, _previewChallenge, GuildTributeChallenge, extensionObject |
| `Protos.GetGuildInviteStatusRequest` | 1 | extensionObject |
| `Protos.GetGuildInviteStatusResponse` | 3 | _status, Profile, extensionObject |
| `Protos.GetGuildRaidStatusesRequest` | 1 | extensionObject |
| `Protos.GetGuildRaidStatusesResponse` | 5 | _raidStatus, _raidResult, _raidProgress, RaidRewardStatus, extensionObject |
| `Protos.GetGuildRequest` | 3 | GuildId, IncludeRecentGuildActivityInfo, extensionObject |
| `Protos.GetGuildResponse` | 2 | Guild, extensionObject |
| `Protos.GetGuildsRequest` | 6 | FilterType, StartIndex, Count, SearchCriteria, Name, extensionObject |
| `Protos.GetGuildsResponse` | 4 | _guild, TotalRecords, IncludeStarterGuild, extensionObject |
| `Protos.GetMapStatsRequest` | 4 | TerritoryMapId, MapStatId, EventType, extensionObject |
| `Protos.GetMapStatsResponse` | 3 | _currentStat, Context, extensionObject |
| `Protos.GetModFiltersRequest` | 1 | extensionObject |
| `Protos.GetModFiltersResponse` | 2 | _filter, extensionObject |
| `Protos.GetModLoadoutsRequest` | 1 | extensionObject |
| `Protos.GetModLoadoutsResponse` | 2 | _modLoadoutTab, extensionObject |
| `Protos.GetNucleusPlayerPreviewRequest` | 3 | NucleusAccessToken, AccessToken, extensionObject |
| `Protos.GetNucleusPlayerPreviewResponse` | 7 | PlayerName, PlayerLevel, PlayerPortraitId, TerritoryTournamentLeagueId, PlayerId, NucleusId, extensionObject |
| `Protos.GetPlayerTributesRequest` | 1 | extensionObject |
| `Protos.GetPlayerTributesResponse` | 2 | PlayerTribute, extensionObject |
| `Protos.GetRoomsRequest` | 1 | extensionObject |
| `Protos.GetRoomsResponse` | 2 | _room, extensionObject |
| `Protos.GetSeasonStatusRequest` | 3 | SeasonId, EventInstanceId, extensionObject |
| `Protos.GetSeasonStatusResponse` | 2 | SeasonStatus, extensionObject |
| `Protos.GetSquadPresetsRequest` | 2 | CombatType, extensionObject |
| `Protos.GetSquadPresetsResponse` | 3 | _squadPresetTab, _fleetPresetTab, extensionObject |
| `Protos.GetTerritoryMapStatusesRequest` | 3 | EventType, _cachedUnitSnapshotMapId, extensionObject |
| `Protos.GetTerritoryMapStatusesResponse` | 5 | _territoryBattleStatus, _territoryBattleResult, _territoryWarStatus, _territoryTournamentEventStatus, extensionObject |
| `Protos.GetTournamentMatchAwayStatusRequest` | 3 | TerritoryMapId, MatchId, extensionObject |
| `Protos.GetTournamentMatchAwayStatusResponse` | 2 | AwayStatus, extensionObject |
| `Protos.GetWarSquadStatsRequest` | 7 | TerritoryMapId, ZoneId, ZoneInstance, SquadId, EventType, TournamentMatchId, extensionObject |
| `Protos.GetWarSquadStatsResponse` | 2 | WarSquad, extensionObject |
| `Protos.GetWebStoreBucketRequest` | 3 | StoreItemId, AcceptLanguage, extensionObject |
| `Protos.GetWebStoreBulkBucketsRequest` | 2 | _requests, extensionObject |
| `Protos.GetWebStoreBulkBucketsResponse` | 2 | _buckets, extensionObject |
| `Protos.GiftRecipientCount` | 3 | RecipientLimitId, RecipientCount, extensionObject |
| `Protos.GraduatingTerritoryReward` | 3 | DefendNumber, Reward, extensionObject |
| `Protos.Guild` | 19 | Profile, _member, _inviteStatus, _raidStatus, Inventory, _raidResult, Progress, _territoryBattleStatus, _guildEvents, _territoryBattleResult, _territoryWarStatus, _roomAvailable, NextChallengesRefresh, _stat, _recentRaidResult, _recentTerritoryWarResult, _lastRaidPointsSummary, _recentTerritoryBattleResult, extensionObject |
| `Protos.GuildBannerColorDef` | 5 | Id, TintColor, BackgroundColor, Hidden, extensionObject |
| `Protos.GuildBannerDef` | 4 | _logo, _color, _logoDefinition, extensionObject |
| `Protos.GuildBannerLogoDef` | 5 | Id, Logo, UiLogo, Hidden, extensionObject |
| `Protos.GuildChallenge` | 9 | ChallengeId, StartTimestamp, EndTimestamp, ActionLink, RewardType, _tier, _participant, DescKey, extensionObject |
| `Protos.GuildChallengeParticipant` | 7 | PlayerId, PlayerName, Contribution, Rank, _currentReward, _nextReward, extensionObject |
| `Protos.enum GuildChallengeRewardType` | 3 | value__, GUILDCHALLENGEREWARDRANKED, GUILDCHALLENGEREWARDCOUNT |
| `Protos.GuildChallengeTier` | 6 | ContributionTotal, ContributionGoal, Status, NameKey, DescKey, extensionObject |
| `Protos.enum GuildChallengeTierStatus` | 4 | value__, GUILDCHALLENGETIERLOCKED, GUILDCHALLENGETIERACTIVE, GUILDCHALLENGETIERCOMPLETED |
| `Protos.GuildEventTracker` | 4 | DefinitionId, CompletedStars, EndTime, extensionObject |
| `Protos.GuildExchangeItem` | 2 | Item, extensionObject |
| `Protos.enum GuildFilterType` | 5 | value__, ALLYGUILDS, RECOMMENDEDGUILDS, SEARCHGUILDSBYNAME, SEARCHGUILDSBYCRITERIA |
| `Protos.GuildIndexEvent` | 15 | Id, Name, Size, Power, AvgMemberPower, Open, InviteOnly, IndexTime, LastActivityTime, CountryCode, LastRaidStartTime, RosterDaysInGame, LastDarkSideTbId, LastLightSideTbId, extensionObject |
| `Protos.GuildInventory` | 3 | _currencyItem, _currentDailyLimit, extensionObject |
| `Protos.GuildInviteStatus` | 12 | GuildId, ExpireTime, PlayerId, Type, LeaderUnit, SentTime, PlayerLevel, PlayerName, LastActiveTime, PlayerPortraitId, TerritoryTournamentLeagueId, extensionObject |
| `Protos.enum GuildInviteType` | 3 | value__, GUILDJOINREQUEST, GUILDINVITATION |
| `Protos.enum GuildJoinType` | 3 | value__, RECOMMENDED, ALLY |
| `Protos.GuildLeaderboard` | 4 | _guild, ResetTimestamp, LeaderboardId, extensionObject |
| `Protos.GuildLeaderboardId` | 4 | LeaderboardType, DefId, MonthOffset, extensionObject |
| `Protos.GuildLeaderboardRequest` | 4 | Count, IncludePlayerGuild, _leaderboardId, extensionObject |
| `Protos.GuildLeaderboardResponse` | 2 | _leaderboard, extensionObject |
| `Protos.enum GuildLeaderboardType` | 7 | value__, GUILDRAIDALLCOMPPTS, GUILDRAIDCOMPPTS, GUILDGALACTICPOWER, GUILDTERRITORYBATTLESTARS, GUILDTERRITORYWAROPPONENTGALACTICPOWER, GUILDRAIDHIGHWATERMARK |
| `Protos.GuildMember` | 19 | PlayerId, PlayerName, PlayerLevel, MemberLevel, GuildXp, LastActivityTime, SquadPower, _memberContribution, GuildJoinTime, GalacticPower, PlayerTitle, PlayerPortrait, _seasonStatus, LifetimeSeasonScore, LeagueId, ShipGalacticPower, CharacterGalacticPower, NucleusId, extensionObject |
| `Protos.GuildMemberContribution` | 4 | Type, CurrentValue, LifetimeValue, extensionObject |
| `Protos.enum GuildMemberLevel` | 5 | value__, GUILDPENDING, GUILDMEMBER, GUILDOFFICER, GUILDLEADER |
| `Protos.GuildOverviewData` | 7 | Id, Name, BannerColorId, BannerLogoId, MemberLevel, CurrentTribute, extensionObject |
| `Protos.GuildProfile` | 25 | Id, Name, ExternalMessageKey, _messageCriteriaKey, LogoBackground, EnrollmentStatus, Trophy, MemberCount, MemberMax, Level, Rank, LevelRequirement, RaidWin, LeaderboardScore, BannerColorId, BannerLogoId, GuildGalacticPower, ChatChannelId, GuildType, _raidLaunchConfig, _guildEventTracker, GuildGalacticPowerForRequirement, SingleLaunchConfig, AutoLaunchConfig, … (+1) |
| `Protos.GuildProgress` | 2 | _raidProgress, extensionObject |
| `Protos.GuildRaid` | 20 | Id, Priority, NameKey, SummaryKey, DescKey, Image, SquadType, CampaignElementIdentifier, IconPrefab, PromotionKey, PortraitIcon, RaidSim, InfoImage, Lockout, LegacyRaid, _currencyReward, GuildRewardsRequireAttempt, AudioVictoryKey, AudioDefeatKey, extensionObject |
| `Protos.GuildRaidActivateRequest` | 4 | GuildRaidId, Identifier, SingleLaunchConfig, extensionObject |
| `Protos.GuildRaidActivateResponse` | 5 | InventoryDelta, RaidStatus, Inventory, Profile, extensionObject |
| `Protos.GuildRaidClaimImmediateRewardRequest` | 3 | RaidId, _regularImmediateRewardIndex, extensionObject |
| `Protos.GuildRaidClaimImmediateRewardResponse` | 2 | Delta, extensionObject |
| `Protos.GuildRaidConfig` | 6 | Id, RaidType, RosterRefreshType, RosterRefresh, RaidDuration, extensionObject |
| `Protos.GuildRaidGlobalConfig` | 3 | GuildRaidLockout, LateEntryParticipationLockoutSeconds, extensionObject |
| `Protos.GuildRaidLaunchConfig` | 10 | RaidId, CampaignMissionIdentifier, AutoLaunch, AutoLaunchImmediately, AutoLaunchTime, JoinPeriodDuration, AutoSimEnabled, Immediate, ScheduledUtcOffsetSeconds, extensionObject |
| `Protos.GuildRaidLockout` | 3 | LockoutStartTime, LockoutEndTime, extensionObject |
| `Protos.GuildRaidMember` | 4 | PlayerId, MemberProgress, MemberRank, extensionObject |
| `Protos.GuildRaidOverviewData` | 13 | AttemptsUsed, TotalAttempts, TallyEndTime, GuildRewardScore, IconPrefab, ExpireTime, GuildRaidTokens, RewardEarnedCrateId, ClaimableImmediateRewardCount, PortraitIcon, RaidId, Identifier, extensionObject |
| `Protos.GuildRaidPointsSummary` | 3 | Identifier, TotalPoints, extensionObject |
| `Protos.GuildRaidProgress` | 4 | Identifier, Progress, WinCount, extensionObject |
| `Protos.GuildRaidResult` | 12 | RaidId, Identifier, Duration, Progress, _raidMember, EndTime, Outcome, _encounterProgress, GuildRewardScore, CachedRewards, ParticipationLockoutTime, extensionObject |
| `Protos.GuildRaidRewardStatus` | 3 | ResetTime, _ineligibleIdentifier, extensionObject |
| `Protos.GuildRaidRosterRefresh` | 3 | Type, _value, extensionObject |
| `Protos.enum GuildRaidRosterRefreshType` | 5 | value__, RAIDROSTERREFRESHNONE, RAIDROSTERREFRESHSCHEDULED, RAIDROSTERREFRESHPURCHASABLE, RAIDROSTERREFRESHTIMED |
| `Protos.GuildRaidSim` | 4 | _simReward, CampaignElementIdentifier, RequirementId, extensionObject |
| `Protos.GuildRaidStatus` | 24 | RaidId, ExpireTime, _opponentUnitState, _playerUnitState, Identifier, InstanceId, RosterRefreshTime, HasPlayerParticipated, RaidState, TallyEndTime, EligibleForRewards, ActivateTimeMs, ForbidLateEntry, JoinPeriodEndTimeMs, _raidMember, EncounterId, LastEncounterIdPlayerParticipated, CurrentEncounterPointsScored, GuildRewardScore, _ineligibleMemberId, _claimedImmediateRewards, CachedRewards, _exhaustedDatacronId, extensionObject |
| `Protos.enum GuildRaidType` | 3 | value__, RAIDTYPEPERMANENT, RAIDTYPETIMELIMITED |
| `Protos.GuildReportEvent` | 13 | Id, GuildId, GuildName, ReportCount, ReporterGuildId, ReporterGuildName, ReporterPlayerId, ReporterPlayerName, EventInstanceId, Details, Timestamp, ReportType, extensionObject |
| `Protos.GuildStat` | 3 | Id, Value, extensionObject |
| `Protos.GuildTerritoryEndEvent` | 2 | _zoneDocumentId, extensionObject |
| `Protos.GuildTributeChallenge` | 3 | PlayerCurrent, GuildCurrent, extensionObject |
| `Protos.enum HeatmapEventType` | 3 | value__, DEFAULT, PURCHASE |
| `Protos.HeatmapMQEvent` | 4 | EventType, CountryCode, DollarAmount, extensionObject |
| `Protos.HelpEntry` | 3 | Id, _tile, extensionObject |
| `Protos.HelpTile` | 4 | TextKey, Image, TitleKey, extensionObject |
| `Protos.enum HelpType` | 4 | value__, BATTLE, SQUAD, PROGRESS |
| `Protos.HeroBanner` | 14 | Id, NameKey, NameValue, DescriptionKey, DescriptionValue, PromoTextKey, PromoTextValue, CharacterImage, BackgroundImage, BundleId, PackId, EndTime, BannerType, extensionObject |
| `Protos.enum HeroBannerType` | 3 | value__, DEFAULTVIEW, PREVIEW |
| `Protos.IAllyListRpc` | 0 | — |
| `Protos.IAuthRpc` | 0 | — |
| `Protos.IBattleRpc` | 0 | — |
| `Protos.IChallengeRpc` | 0 | — |
| `Protos.IChannelRpc` | 0 | — |
| `Protos.IChatManagementRpc` | 0 | — |
| `Protos.IConquestRpc` | 0 | — |
| `Protos.IContentRpc` | 0 | — |
| `Protos.ICraftingRpc` | 0 | — |
| `Protos.IDatacronRpc` | 0 | — |
| `Protos.IGameChannelRpc` | 0 | — |
| `Protos.IGameRpc` | 0 | — |
| `Protos.IGuildRpc` | 0 | — |
| `Protos.ILeaderboardRpc` | 0 | — |
| `Protos.ILightspeedTokenRpc` | 0 | — |
| `Protos.ILoyaltyRpc` | 0 | — |
| `Protos.IMatchMakingRpc` | 0 | — |
| `Protos.IPlayerRpc` | 0 | — |
| `Protos.ISagaRpc` | 0 | — |
| `Protos.ISeasonRpc` | 0 | — |
| `Protos.ISessionRpc` | 0 | — |
| `Protos.ISingularCampaignRpc` | 0 | — |
| `Protos.IStatModRpc` | 0 | — |
| `Protos.IStoreRpc` | 0 | — |
| `Protos.ITelemetryRpc` | 0 | — |
| `Protos.ITerritoryRpc` | 0 | — |
| `Protos.IUnitRpc` | 0 | — |
| `Protos.ImmediatePurchaseStoreItem` | 3 | Currency, _packs, extensionObject |
| `Protos.Inbox` | 2 | _message, extensionObject |
| `Protos.enum InboxAction` | 3 | value__, ACCEPT, REJECT |
| `Protos.InboxAlly` | 13 | PlayerName, PlayerLevel, LeaderUnitDefinitionId, LeaderUnitLevel, LeaderUnitTier, PlayerId, GuildName, GuildBannerColorId, GuildBannerLogoId, GuildTypeId, Portrait, LeagueId, extensionObject |
| `Protos.InboxGuild` | 10 | Name, Level, Logo, LogoBackground, Rank, MemberCount, MemberMax, GuildBannerColor, GuildBannerLogo, extensionObject |
| `Protos.InboxMessage` | 37 | Id, SubjectKey, SubjectValue, MessageKey, MessageValue, ExpireTime, _attachment, Type, SourceKey, SourceValue, StartTime, Ally, StoreReceipt, FromSupport, Rank, Guild, ActionLink, Score, _statMod, _playerValueEntry, OpponentScore, DeletableOnActionLink, ChatName, CustomButtonKey, … (+13) |
| `Protos.InboxMessageEntitlement` | 5 | StoreItemId, EntitlementId, TransactionId, GifterPlayerName, extensionObject |
| `Protos.InboxMessageEvent` | 9 | TemplateId, RecipientId, MessageId, RequesterId, ChatName, ChatInviter, _messageParam, SourceKey, extensionObject |
| `Protos.InboxMessageLocParam` | 3 | Key, Value, extensionObject |
| `Protos.InboxMessageStoreCoupon` | 3 | OfferId, DiscountPercentage, extensionObject |
| `Protos.enum InboxMessageType` | 30 | value__, BASIC, ALLYREQUEST, STORERECEIPT, RANKREWARD, GUILDINVITE, JOINGUILDREQUEST, GUILDRANKEDREWARD, GUILDEXCHANGEAWARD, GAMEEVENTREWARD, TERRITORYBATTLECOMPLETEREWARD, TERRITORYCOVERTMISSIONREWARD, TERRITORYWARCOMPLETEREWARD, CHATROOMINVITATION, TERRITORYTOURNAMENTMATCHREWARD, SEASONENDREWARD, TERRITORYTOURNAMENTEVENTREWARD, PARAMETERIZED, TERRITORYTOURNAMENTEVENTSIGNUP, TERRITORYTOURNAMENTDAILYRANKREWARD, GENERICRECEIPT, WEBSTOREPURCHASE, WEBSTORERECEIPT, GIFTENTITLEMENT, … (+6) |
| `Protos.InboxPlayerValueEntry` | 4 | PlayerId, PlayerName, Value, extensionObject |
| `Protos.InboxStoreReceipt` | 4 | StoreItemId, StoreItemNameKey, StoreItemRemaining, extensionObject |
| `Protos.InboxUpdate` | 3 | _added, _removedId, extensionObject |
| `Protos.InitialDataRequest` | 2 | AppStart, extensionObject |
| `Protos.InitialDataResponse` | 63 | Id, Player, _swrveAbVariants, _configOverride, Inventory, _gameEvent, _interstitialMessage, Inbox, _activeMessage, Store, PurchaseStats, _dailyLogin, _challengeProgress, ChallengeResetTime, OnboardingCharacterSquad, OnboardingShipSquad, AllyList, _cooldownStatus, _dailyActionCapStatus, _onboardingSocialPackUnitId, _war, _subscriptionStatus, Guild, InviteStatus, … (+39) |
| `Protos.InterstitialMessage` | 15 | Id, Type, TitleKey, DescKey, Image, ActionLink, StartTime, EndTime, Priority, VisibilityRequirement, TickerMessage, NewsletterMessage, _trigger, ActionLinkDef, extensionObject |
| `Protos.enum InterstitialMessageType` | 4 | value__, TICKER, NEWSLETTER, HOMEPROMO |
| `Protos.InterstitialTrigger` | 3 | Id, InterstitialTriggerType, extensionObject |
| `Protos.enum InterstitialTriggerType` | 4 | value__, EVENT, CHALLENGE, STORE |
| `Protos.Inventory` | 9 | _unit, _squad, _shipSquad, _material, _currencyItem, _equipment, _unequippedMod, _lightspeedToken, extensionObject |
| `Protos.InventoryDelta` | 5 | Credit, Debit, ReasonCode, ReasonDetail, extensionObject |
| `Protos.enum InventoryDeltaReason` | 76 | value__, SILENT, CHEAT, BATTLESTART, BATTLEFINISH, PLAYERLEVELUPBONUS, ALLYGIFTAWARD, STOREPURCHASE, CRAFTITEMS, TRAINUNIT, PROMOTEUNIT, SELLITEMS, INBOXMESSAGE, SCOREBONUS, ENTERKEYCARDEVENT, DAILYLOGINAWARD, CHALLENGEAWARD, ITEMCONVERSION, SHIPMENTREFRESH, ENERGYREFRESH, ENABLEDPUSHNOTES, SHIPMENTSPVP, SHIPMENTSPVE, SKILLUPGRADE, … (+52) |
| `Protos.InventoryDeltaSegment` | 30 | _currencyItem, _unit, _material, _itemConversion, _equipment, _energyBonus, _statMod, _statModOverflow, SourceItemType, _unlockedPlayerTitle, _unlockedPlayerPortrait, _seasonPoint, _conquestPoint, _conquestConsumable, _conquestArtifact, BattlePassActivated, _dailyLogin, ConquestStaminaRegenTimeReductionIncrement, ConquestEnergyRegenTimeReductionIncrement, ConquestFreeArtifactSwapActivated, _datacron, _datacronOverflow, ConquestBattlePassIdSource, _episodeXp, … (+6) |
| `Protos.InventoryLogEvent` | 9 | Player, RecordType, RefId, _debitItem, _creditItem, Timestamp, ExpireTimestamp, RefData, extensionObject |
| `Protos.InventoryUpdate` | 3 | _delta, PlayerUpdate, extensionObject |
| `Protos.InviteToGuildRequest` | 2 | _playerId, extensionObject |
| `Protos.InviteToGuildResponse` | 3 | _status, Profile, extensionObject |
| `Protos.ItemConversionDetail` | 4 | MessageKey, SourceItem, _convertedItem, extensionObject |
| `Protos.enum ItemType` | 33 | value__, EMPTY, UNIT, CURRENCY, BUCKET, XP, MATERIAL, ENERGY, EQUIPMENT, EVENTKEYCARD, POWERUPBUNDLE, MYSTERYBOX, STATMOD, MYSTERYSTATMOD, PLAYERTITLE, STATMODPARAMETERIZED, PLAYERPORTRAIT, SEASONPOINT, GALACTICBUNDLE, CONQUESTPOINT, ARTIFACT, CONQUESTCONSUMABLE, BATTLEPASS, LOGINCALENDAR, … (+9) |
| `Protos.JoinConquestRequest` | 3 | DifficultyType, GameEventId, extensionObject |
| `Protos.JoinConquestResponse` | 3 | Status, _storeItemPurchaseResetId, extensionObject |
| `Protos.JoinGuildRaidRequest` | 3 | CampaignMissionIdentifier, RaidId, extensionObject |
| `Protos.JoinGuildRaidResponse` | 2 | RaidStatus, extensionObject |
| `Protos.JoinGuildRequest` | 4 | GuildId, JoinType, NewLeaderId, extensionObject |
| `Protos.JoinGuildResponse` | 4 | Status, Guild, Player, extensionObject |
| `Protos.JoinSeasonRequest` | 3 | SeasonId, EventInstanceId, extensionObject |
| `Protos.JoinSeasonResponse` | 2 | SeasonStatus, extensionObject |
| `Protos.JoinStarterGuildRequest` | 2 | NewLeaderId, extensionObject |
| `Protos.JoinStarterGuildResponse` | 3 | Guild, Player, extensionObject |
| `Protos.JoinTerritoryEventRequest` | 3 | TerritoryMapId, EventType, extensionObject |
| `Protos.JoinTerritoryEventResponse` | 4 | _territoryBattleStatus, _territoryWarStatus, _territoryTournamentEventStatus, extensionObject |
| `Protos.enum LanguageType` | 18 | value__, CHSCN, CHTCN, DANDK, DUTNL, ENGUS, FINFI, FREFR, GERDE, ITAIT, JPNJP, KORKR, NORNO, PORBR, RUSRU, SPAES, SPAMX, SWESE |
| `Protos.LeaderboardData` | 4 | Id, _player, PlayerStatus, extensionObject |
| `Protos.LeaderboardGuild` | 6 | Id, Name, BannerColorId, BannerLogoId, GuildTypeId, extensionObject |
| `Protos.LeaderboardPlayer` | 15 | Id, Name, Level, Guild, IsFake, PvpStatus, _squadUnit, Title, Power, Portrait, SeasonsLifetimeScore, SeasonLeagueId, PlayerRating, Tier, extensionObject |
| `Protos.LeaderboardRequest` | 8 | CombatType, EventId, EventInstanceId, GroupId, LeaderboardType, League, Division, extensionObject |
| `Protos.LeaderboardResponse` | 4 | _player, PlayerStatus, _leaderboard, extensionObject |
| `Protos.LeaderboardStatus` | 6 | Rank, RankDelta, Score, ScoreDelta, Tier, extensionObject |
| `Protos.enum LeaderboardType` | 13 | value__, LEADERBOARDNONE, LEADERBOARDPVPARENA, LEADERBOARDEVENT, LEADERBOARDTERRITORYTOURNAMENTGROUP, LEADERBOARDSEASONGLOBAL, LEADERBOARDSEASONDIVISION, LEADERBOARDSEASONPLAYERDIVISION, LEADERBOARDTERRITORYTOURNAMENTGUILD, LEADERBOARDTERRITORYTOURNAMENTALLY, LEADERBOARDCOLISEUMGLOBAL, LEADERBOARDCOLISEUMGUILD, LEADERBOARDCOLISEUMSHARD |
| `Protos.LeagueDivisionRewardPreview` | 4 | Division, _primaryReward, _detailedReward, extensionObject |
| `Protos.LeaveGuildRequest` | 2 | NewLeaderId, extensionObject |
| `Protos.LeaveGuildResponse` | 1 | extensionObject |
| `Protos.LeaveRoomRequest` | 2 | RoomId, extensionObject |
| `Protos.LeaveRoomResponse` | 1 | extensionObject |
| `Protos.LeaveTerritoryEventRequest` | 3 | TerritoryMapId, EventType, extensionObject |
| `Protos.LeaveTerritoryEventResponse` | 4 | _territoryBattleStatus, _territoryWarStatus, _territoryTournamentEventStatus, extensionObject |
| `Protos.LevelModRequest` | 5 | ModId, _modConsumedId, ModSessionId, ModLevelCount, extensionObject |
| `Protos.LevelModResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.LevelUpEraUnitRequest` | 3 | UnitId, Level, extensionObject |
| `Protos.LevelUpEraUnitResponse` | 3 | Delta, EraStatus, extensionObject |
| `Protos.LightspeedToken` | 4 | Id, ExpireTime, DefinitionId, extensionObject |
| `Protos.LightspeedTokenDefinition` | 18 | Id, UnitUpgrade, _includeTag, _excludeTag, ReleaseEraTag, _dismantlingReward, NameKey, DescriptionKey, TokenTierLargeSprite, TokenTierSmallSprite, TokenImage, CombatType, _applicableUnitDefs, _lookupMission, _raidLookup, _actionLinkLookup, _raidImmediateLookup, extensionObject |
| `Protos.LinkAccountRequest` | 2 | SourceLinkCode, extensionObject |
| `Protos.LinkAccountResponse` | 1 | extensionObject |
| `Protos.LinkEAConnectConfirmRequest` | 5 | Telephone, TelephoneCountry, Email, Code, extensionObject |
| `Protos.LinkEAConnectConfirmResponse` | 2 | NucleusId, extensionObject |
| `Protos.LinkEAConnectRequest` | 6 | Telephone, TelephoneCountry, Email, AcceptLanguage, DateOfBirth, extensionObject |
| `Protos.LinkEAConnectResponse` | 1 | extensionObject |
| `Protos.LinkNewEAConnectRequest` | 3 | AccessCode, AccessToken, extensionObject |
| `Protos.LinkNucleusAccountRequest` | 2 | NucleusAccessToken, extensionObject |
| `Protos.LinkNucleusAccountResponse` | 1 | extensionObject |
| `Protos.LinkedStoreItem` | 5 | LinkId, _storeItem, Type, _contextualStoreItem, extensionObject |
| `Protos.enum LinkedStoreItemType` | 5 | value__, LINKEDSTOREITEMUNIT, LINKEDSTOREITEMEVENT, LINKEDSTOREITEMRAID, LINKEDSTOREITEMPREVIEW |
| `Protos.LoadingScreen` | 5 | Id, TextureKey, TipKey, ForceDisplayFirst, extensionObject |
| `Protos.LoanedUnitEraDefinition` | 3 | Id, ExpireTime, extensionObject |
| `Protos.LocalizationBundleRequest` | 2 | Id, extensionObject |
| `Protos.LocalizationBundleResponse` | 2 | LocalizationBundle, extensionObject |
| `Protos.LocalizationParam` | 3 | ParamKey, ParamValue, extensionObject |
| `Protos.LockModRequest` | 2 | ModId, extensionObject |
| `Protos.LockModResponse` | 1 | extensionObject |
| `Protos.LockScavengerItemRequest` | 4 | ClearLocks, _removeLock, _addLock, extensionObject |
| `Protos.LockScavengerItemResponse` | 2 | _scavengerLockedItemId, extensionObject |
| `Protos.LookupActionLink` | 3 | DeskKey, ActionLink, extensionObject |
| `Protos.LookupMission` | 3 | MissionIdentifier, _requirementId, extensionObject |
| `Protos.MandatoryDatacron` | 4 | MinLevel, SetId, TemplateId, extensionObject |
| `Protos.MandatoryRosterUnit` | 3 | Id, Slot, extensionObject |
| `Protos.MatchMakingRequest` | 4 | CombatType, EventId, EventInstanceId, extensionObject |
| `Protos.MatchMakingResponse` | 4 | _opponent, UsingTutorialArenaOpponents, PlayerStatus, extensionObject |
| `Protos.enum MatchState` | 11 | value__, MATCHSTATESETUP, MATCHSTATEATTACK, MATCHSTATECOMPLETE, MATCHSTATECREATED, MATCHSTATECOMPLETEWIN, MATCHSTATECOMPLETELOSS, MATCHSTATECOMPLETEWINTIE, MATCHSTATECOMPLETELOSSTIE, MATCHSTATECOMPLETEWINBYE, MATCHSTATECOMPLETENOACTIVITY |
| `Protos.MatchedPlayer` | 16 | Id, PlayerName, Level, Power, OpponentStatus, ScoreConfig, Squad, GuildName, GuildBannerColorId, GuildBannerLogoId, PlayerTitle, PlayerPortrait, InStarterGuild, _seasonStatus, LeagueId, extensionObject |
| `Protos.MatchedPlayerScoreConfig` | 3 | PointsPerEnemyDefeated, PointsPerAllySurvivor, extensionObject |
| `Protos.enum MaterialType` | 17 | value__, CRAFTINGMATERIAL, XPMATERIAL, UNITSHARDMATERIAL, PROMOTIONMATERIAL, SKILLMATERIAL, SELLMATERIAL, SHIPXPMATERIAL, SHIPSKILLMATERIAL, STATMODSLICINGMATERIAL, STATMODOVERCLOCKINGMATERIAL, RELICMATERIAL, SCRAPMATERIAL, SCRAPSURPLUS, DATACRONMATERIAL, TRACKINGMATERIAL, ERAUPGRADEMATERIAL |
| `Protos.enum MemberContributionType` | 4 | value__, CONTRIBUTIONTYPECOMMENDATION, CONTRIBUTIONTYPETRIBUTE, CONTRIBUTIONTYPEDONATION |
| `Protos.enum MemoryLevel` | 4 | value__, MEMORYLOW, MEMORYMEDIUM, MEMORYHIGH |
| `Protos.MessageContextData` | 2 | _rewardPreview, extensionObject |
| `Protos.MessageDialog` | 3 | TitleKey, DescKey, extensionObject |
| `Protos.MessageParam` | 5 | Key, Value, _paramValue, FormatType, extensionObject |
| `Protos.MessageParamData` | 3 | Key, _param, extensionObject |
| `Protos.MetadataRequest` | 3 | ClientSpecs, RequestType, extensionObject |
| `Protos.enum MetadataRequestType` | 3 | value__, DEFAULT, CLIENTPARAMS |
| `Protos.MetadataResponse` | 24 | _config, AssetVersion, AssetSubpath, TexFormat, ServerTimestamp, DebugMode, LatestLocalizationBundleVersion, LatestGamedataVersion, ResourceCacheMemoryLimit, GamedataRevision, ChannelServerEnabled, ChannelServerEndpoint, MonoExtraMemoryAllocationDivisor, QualityData, _endpoint, LatestLocalizationRevision, ServerVersion, _clientCache, _featuredCommand, EadpParameters, LatestSegmentedContentVersion, _serverProperty, AssetPath, extensionObject |
| `Protos.MilestoneEvent` | 17 | Id, Title, Subtitle, Rules, RulesSummary, CompletionUnitType, Image, LargeCardImage, LargeCard, Featured, Repeatable, ShowInStore, _tier, _rewardShared, _schedule, RepeatsAllowed, extensionObject |
| `Protos.MilestoneEventProgress` | 4 | EventInstanceId, Progress, _tiersClaimed, extensionObject |
| `Protos.MilestoneEventSchedule` | 6 | ScheduleInstanceId, StartTime, EndTime, ExpirationTime, DisplayStartTime, extensionObject |
| `Protos.MilestoneEventsProgressRequest` | 2 | _eventInstanceId, extensionObject |
| `Protos.MilestoneEventsProgressResponse` | 2 | _progress, extensionObject |
| `Protos.MilestoneEventsRequest` | 3 | AcceptLanguage, ScheduleFilter, extensionObject |
| `Protos.MilestoneEventsResponse` | 2 | _event, extensionObject |
| `Protos.MilestoneTier` | 3 | CompletionCriteriaAmount, _reward, extensionObject |
| `Protos.MissionGrantedAbility` | 11 | AbilityId, TargetRuleId, UnlockRequirement, RankLabelOverrideKey, UseOverlayIcon, OverlaySpriteName, _zonesToApplyTo, ShowInPlatoonView, ModifierData, ConditionDescKey, extensionObject |
| `Protos.ModLoadout` | 4 | Id, Name, _modId, extensionObject |
| `Protos.ModLoadoutTab` | 5 | Id, Name, _modLoadout, Unique, extensionObject |
| `Protos.ModRecommendation` | 6 | Id, _setRecommendation, _primaryStatRecommendation, _secondaryStatRecommendation, TipKey, extensionObject |
| `Protos.ModRecommendationPrimaryStat` | 3 | Slot, _stat, extensionObject |
| `Protos.ModRecommendationSetBonus` | 3 | ModSetId, Quantity, extensionObject |
| `Protos.ModifierItem` | 5 | Type, MaterialType, Id, Category, extensionObject |
| `Protos.ModifierLocation` | 3 | Type, Identifier, extensionObject |
| `Protos.enum ModifierLocationType` | 3 | value__, MODIFIERLOCATIONCAMPAIGN, MODIFIERLOCATIONGALACTICWAR |
| `Protos.MoveSquadPresetRequest` | 5 | CombatType, SourceTabId, SourceIndex, DestTabId, extensionObject |
| `Protos.MoveSquadPresetResponse` | 1 | extensionObject |
| `Protos.enum MoveType` | 7 | value__, MOVETYPEATTACK, MOVETYPELIMITBREAK, MOVETYPECRYSTAL, MOVETYPENONE, MOVETYPELEADERABILITY, MOVETYPECONSUMABLE |
| `Protos.MutePlayerRequest` | 3 | _mutePlayerId, _unmutePlayerId, extensionObject |
| `Protos.MutePlayerResponse` | 1 | extensionObject |
| `Protos.MysteryBoxDefinition` | 10 | Id, Texture, TitleKey, DescKey, DetailsTitleKey, DetailsDescKey, IconTextKey, _previewItem, ShowPrimaryRewards, extensionObject |
| `Protos.MysteryStatModDefinition` | 10 | Id, SlotOld, SetId, MinRarity, MaxRarity, MinTier, MaxTier, _slot, DescKey, extensionObject |
| `Protos.NameSpacesRequest` | 2 | OnlyCompatible, extensionObject |
| `Protos.NameSpacesResponse` | 2 | _nameSpace, extensionObject |
| `Protos.NewsletterMessage` | 4 | NewsletterMessageType, AlertFrequency, MaxAlertCount, extensionObject |
| `Protos.enum NewsletterMessageType` | 3 | value__, TEXT, TEXTWITHTEXTURE |
| `Protos.Offer` | 17 | PaymentCurrency, Price, CostKey, CooldownKey, DailyPurchaseCapKey, InAppProductId, AvailableAtEpoch, CurrencyCode, LocalPrice, CountryCode, FinalTotalAmount, TotalDiscountAmount, TotalDiscountRate, _promotion, AdditionalBucketId, WebProductId, extensionObject |
| `Protos.enum OmicronMode` | 18 | value__, ALLOMICRON, PVEOMICRON, PVPOMICRON, GUILDRAIDOMICRON, TERRITORYSTRIKEOMICRON, TERRITORYCOVERTOMICRON, TERRITORYBATTLEBOTHOMICRON, TERRITORYWAROMICRON, TERRITORYTOURNAMENTOMICRON, WAROMICRON, CONQUESTOMICRON, GALACTICCHALLENGEOMICRON, PVEEVENTOMICRON, TERRITORYTOURNAMENT3OMICRON, TERRITORYTOURNAMENT5OMICRON, GALACTICCHALLENGE3OMICRON, GALACTICCHALLENGE5OMICRON |
| `Protos.enum OperationType` | 3 | value__, ADDITIVE, MULTIPLICATIVE |
| `Protos.OverclockModRequest` | 3 | ModId, ModSessionId, extensionObject |
| `Protos.OverclockModResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.OverviewData` | 7 | ColiseumOverview, GuildOverview, TerritoryBattleOverview, ArenaOverview, GuildRaidOverview, TerritoryWarOverview, extensionObject |
| `Protos.enum ParamFormatType` | 4 | value__, FORMATNONE, FORMATSTANDARDTRUNCATE, FORMATCOMMANODECIMAL |
| `Protos.enum PerformanceLevel` | 4 | value__, PERFORMANCELOW, PERFORMANCEMEDIUM, PERFORMANCEHIGH |
| `Protos.PersistentVfx` | 5 | Id, Prefab, Priority, LineOverride, extensionObject |
| `Protos.PersistentVfxReference` | 2 | Id, extensionObject |
| `Protos.PingRequest` | 1 | extensionObject |
| `Protos.PingResponse` | 2 | SystemTime, extensionObject |
| `Protos.Player` | 41 | CharacterOnboardingComplete, Xp, Level, _energy, Name, AmountSpent, ShipOnboardingComplete, NameChangeCount, FreeNameChangeCount, LastGuildJoinTime, SynergyId, SynergySellId, AllyCode, EulaAccepted, Age, LocalTimeZoneOffsetMinutes, DebugTimeOffset, LastLoginTime, LastSpentTime, PushNotesEnabled, _lastEnergyBonusTime, CountryCode, JoinTime, _featureSuspended, … (+17) |
| `Protos.PlayerArenaProfileRequest` | 4 | PlayerId, AllyCode, PlayerDetailsOnly, extensionObject |
| `Protos.PlayerArenaProfileResponse` | 10 | Name, Level, AllyCode, PlayerId, _pvpProfile, LocalTimeZoneOffsetMinutes, LifetimeSeasonScore, PlayerRating, NucleusId, extensionObject |
| `Protos.PlayerIndexEvent` | 5 | PlayerId, PlayerName, FakePlayer, PlayerNucleusId, extensionObject |
| `Protos.PlayerPortrait` | 2 | Id, extensionObject |
| `Protos.PlayerPortraitDefinition` | 7 | Id, NameKey, DescKey, Obtainable, Hidden, Icon, extensionObject |
| `Protos.PlayerPreference` | 9 | _commandMessages, _favoriteUnitDefIds, _entries, _mutedPlayer, _reportedPlayer, Category, _allowedModRecommendationUnitBaseId, _favoriteDatacronIds, extensionObject |
| `Protos.PlayerPreferenceEntry` | 3 | Key, Value, extensionObject |
| `Protos.PlayerPreviewRequest` | 1 | extensionObject |
| `Protos.PlayerPreviewResponse` | 4 | PlayerId, Name, Level, extensionObject |
| `Protos.PlayerProfileRequest` | 4 | PlayerId, AllyCode, NucleusId, extensionObject |
| `Protos.PlayerProfileResponse` | 27 | Name, Level, AllyCode, PlayerId, _rosterUnit, _profileStat, GuildId, GuildName, GuildLogoBackground, GuildBannerColor, GuildBannerLogo, _pvpProfile, SelectedPlayerTitle, _unlockedPlayerTitle, GuildTypeId, LocalTimeZoneOffsetMinutes, LastActivityTime, SelectedPlayerPortrait, _unlockedPlayerPortrait, _seasonStatus, LifetimeSeasonScore, PlayerRating, _datacron, NucleusId, … (+3) |
| `Protos.PlayerProfileStat` | 5 | NameKey, VersionKey, Value, Index, extensionObject |
| `Protos.enum PlayerProfileTab` | 4 | value__, PROFILEPVPCHARACTER, PROFILEPVPSHIP, PROFILEPVPTOURNAMENT |
| `Protos.PlayerPurchase` | 3 | ItemId, Count, extensionObject |
| `Protos.PlayerPurchasesRequest` | 2 | LimitedPurchasesOnly, extensionObject |
| `Protos.PlayerPurchasesResponse` | 2 | _purchases, extensionObject |
| `Protos.PlayerPvpProfile` | 5 | Tab, Rank, Squad, EventId, extensionObject |
| `Protos.PlayerRankStatus` | 3 | LeagueId, DivisionId, extensionObject |
| `Protos.PlayerRating` | 3 | PlayerSkillRating, PlayerRankStatus, extensionObject |
| `Protos.PlayerSkillRating` | 2 | SkillRating, extensionObject |
| `Protos.PlayerTitle` | 2 | Id, extensionObject |
| `Protos.PlayerTitleDefinition` | 7 | Id, NameKey, DescKey, Obtainable, Hidden, ShortDescKey, extensionObject |
| `Protos.PlayerUpdate` | 4 | GuildId, HasGuildJoinRequest, GuildType, extensionObject |
| `Protos.PlayerUpdateRequest` | 7 | Name, EulaAccepted, Age, PushNotesEnabled, PassesCoppa, AgeUpEulaAccepted, extensionObject |
| `Protos.PlayerUpdateResponse` | 3 | InventoryDelta, Adult, extensionObject |
| `Protos.PointAtElement` | 26 | ElementId, OffsetX, OffsetY, Rotation, TextOffsetX, TextOffsetY, TextOffsetAnchor, Update, Clamp, RotationBehavior, DelaySeconds, ShowArrow, ShowHighlight, ImageSource, ImageWidth, ImageHeight, ImageOffsetX, ImageOffsetY, PointerType, TapDismiss, ScrimVisible, ScrimBlocking, ShowText, ShowBartender, … (+2) |
| `Protos.enum PointAtRotationBehavior` | 4 | value__, ROTATIONBEHAVIORNONE, ROTATIONBEHAVIORAUTOFROMCENTER, ROTATIONBEHAVIORAUTOFROMCENTERIFCLAMPED |
| `Protos.enum PointerType` | 4 | value__, POINT, CLICK, SWIPE |
| `Protos.Position` | 4 | X, Y, Z, extensionObject |
| `Protos.PowerUpBundle` | 5 | Id, TargetLevel, TargetGearTier, AutoSelectTier, extensionObject |
| `Protos.PrepareGiftPurchaseRequest` | 3 | StoreItemId, RecipientPlayerId, extensionObject |
| `Protos.PrepareGiftPurchaseResponse` | 2 | TransactionId, extensionObject |
| `Protos.PrepareWebStoreDiscountDeeplinkRequest` | 2 | CouponId, extensionObject |
| `Protos.PrepareWebStoreDiscountDeeplinkResponse` | 1 | extensionObject |
| `Protos.PromoteMemberRequest` | 3 | PlayerId, Level, extensionObject |
| `Protos.PromoteMemberResponse` | 2 | Member, extensionObject |
| `Protos.PromoteModRequest` | 3 | ModId, ModSessionId, extensionObject |
| `Protos.PromoteModResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.PromoteRelicRequest` | 3 | UnitId, TargetTier, extensionObject |
| `Protos.PromoteRelicResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.PromoteUnitRequest` | 2 | UnitId, extensionObject |
| `Protos.PromoteUnitResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.Promotion` | 6 | DiscountAmount, DiscountRate, Usage, StartDate, EndDate, extensionObject |
| `Protos.enum PromotionRelegationStatus` | 4 | value__, PROMOTIONRELEGATIONSTATUSNONE, PROMOTIONRELEGATIONSTATUSPROMOTION, PROMOTIONRELEGATIONSTATUSRELEGATION |
| `Protos.PublishChannelEventsMessage` | 2 | _event, extensionObject |
| `Protos.PublishChannelEventsRequest` | 2 | _event, extensionObject |
| `Protos.PublishChannelEventsResponse` | 1 | extensionObject |
| `Protos.PublishChannelMessageRequest` | 9 | ChannelId, CorrelationId, Message, EventCount, LastUpdateTimestamp, LastUpdateInterval, QueryEventsInChannel, _data, extensionObject |
| `Protos.PublishChannelMessageResponse` | 4 | _event, UpdateTimestamp, UpdateInterval, extensionObject |
| `Protos.PurchaseAbilityUnlockRequest` | 3 | UnitId, AbilityId, extensionObject |
| `Protos.PurchaseAbilityUnlockResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.PurchaseContextItem` | 4 | Id, Type, Quantity, extensionObject |
| `Protos.PurchaseShipmentRefreshRequest` | 2 | ShipmentType, extensionObject |
| `Protos.PurchaseShipmentRefreshResponse` | 3 | Result, Shipment, extensionObject |
| `Protos.PurchasedInfo` | 2 | _purchasedStats, extensionObject |
| `Protos.PurchasedStat` | 3 | Id, Quantity, extensionObject |
| `Protos.PushNoteEvent` | 3 | TemplateId, _recipientId, extensionObject |
| `Protos.QueryChannelEventsRequest` | 6 | _channelId, EventCount, MinTimestamp, MaxTimestamp, _channelEventRequest, extensionObject |
| `Protos.QueryChannelEventsResponse` | 2 | _event, extensionObject |
| `Protos.QuestBlockDefinition` | 6 | Id, Title, _questPreview, HeadlineReward, HeadlineQuestCompleteRequirement, extensionObject |
| `Protos.QuestPreview` | 3 | ChallengeId, UnlockTime, extensionObject |
| `Protos.RaidAbilityOverride` | 5 | AbilityId, TargetRuleId, LocKey, Icon, extensionObject |
| `Protos.RaidBlackboardPointSource` | 3 | Id, LocKey, extensionObject |
| `Protos.RaidEvent` | 2 | _guildMemberId, extensionObject |
| `Protos.RaidMember` | 5 | PlayerId, MemberProgress, MemberRank, MemberAttempt, extensionObject |
| `Protos.RaidModifier` | 8 | DescKey, _grantedAbility, ScoreMultiplierUnscaled, RequiredUnitTier, RequiredRelicTier, _abilityOverride, OverrideRelicTier, extensionObject |
| `Protos.enum RaidOutcome` | 7 | value__, RAIDOUTCOMEUNDEFINED, RAIDOUTCOMEWIN, RAIDOUTCOMEEXPIRED, RAIDOUTCOMEINPROGRESS, RAIDOUTCOMEABANDONED, RAIDOUTCOMESIMMED |
| `Protos.RaidPointsStructure` | 3 | _blackboardPointSource, _modifier, extensionObject |
| `Protos.RaidStartActivityData` | 2 | RaidId, extensionObject |
| `Protos.enum RaidState` | 6 | value__, RAIDINPROGRESS, RAIDINTALLY, RAIDCOMPLETE, RAIDEXPIRED, RAIDPENDINGCOMPLETE |
| `Protos.Rank` | 3 | Place, Size, extensionObject |
| `Protos.RankRewardPreview` | 5 | RankStart, RankEnd, _primaryReward, _detailedReward, extensionObject |
| `Protos.enum Rarity` | 9 | value__, ONESTAR, TWOSTAR, THREESTAR, FOURSTAR, FIVESTAR, SIXSTAR, SEVENSTAR, NOSTAR |
| `Protos.RawAssetManifest` | 8 | Version, _records, Platform, TexFormat, Environment, Timestamp, Revision, extensionObject |
| `Protos.RawAssetManifestRecord` | 11 | Name, Version, Size, UncompressedSize, Shared, Rank, PackageType, _entries, _dependencies, Crc, extensionObject |
| `Protos.RawAssetManifestRecordEntry` | 4 | AssetName, RuntimeSize, CloneRuntimeSize, extensionObject |
| `Protos.Recipe` | 10 | Id, DescKey, Result, _ingredients, Type, ViewRequirement, UnlockLevel, CraftRequirement, RequiredEraLevel, extensionObject |
| `Protos.enum RecipeType` | 13 | value__, RECIPEINVALID, RECIPEPROMOTION, RECIPESHIP, RECIPECONSUMABLE, RECIPECHARACTER, RECIPESKILL, RECIPEEQUIPMENT, RECIPEMODSLICING, RECIPEMATERIAL, RECIPERELIC, RECIPEDATACRON, RECIPEERALEVEL |
| `Protos.RecommendedSquad` | 9 | DescriptionKey, Thumbnail, _unitDefId, Name, ShowRequirement, HideRequirement, ActionLink, Id, extensionObject |
| `Protos.RedeemGrindTicketsMultiRequest` | 3 | _grindTicketRequest, Type, extensionObject |
| `Protos.RedeemGrindTicketsRequest` | 5 | CampaignMissionIdentifier, TicketCount, EventId, InstanceId, extensionObject |
| `Protos.RedeemGrindTicketsResponse` | 4 | _inventoryDelta, PlayerUpdate, _warStatus, extensionObject |
| `Protos.RefreshCooldownRequest` | 2 | Id, extensionObject |
| `Protos.RefreshCooldownResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.RefreshShipmentsRequest` | 1 | extensionObject |
| `Protos.RefreshShipmentsResponse` | 2 | _shipment, extensionObject |
| `Protos.RefreshTransactionRequest` | 2 | EnergyType, extensionObject |
| `Protos.RefreshTransactionResponse` | 3 | Player, _inventoryDelta, extensionObject |
| `Protos.RejectJoinGuildRequest` | 2 | PlayerId, extensionObject |
| `Protos.RejectJoinGuildResponse` | 3 | _status, Profile, extensionObject |
| `Protos.Relic` | 2 | CurrentTier, extensionObject |
| `Protos.RelicDefinition` | 6 | _relicTierDefinitionId, UpgradeTableId, AlignmentColorOverride, Texture, NameKey, extensionObject |
| `Protos.enum RelicTier` | 53 | value__, RELICLOCKED, RELICUNLOCKED, RELICTIER01, RELICTIER02, RELICTIER03, RELICTIER04, RELICTIER05, RELICTIER06, RELICTIER07, RELICTIER08, RELICTIER09, RELICTIER10, RELICTIER11, RELICTIER12, RELICTIER13, RELICTIER14, RELICTIER15, RELICTIER16, RELICTIER17, RELICTIER18, RELICTIER19, RELICTIER20, RELICTIER21, … (+29) |
| `Protos.RelicTierDefinition` | 5 | Id, Stat, RelicStatTable, Tier, extensionObject |
| `Protos.RemoveMemberRequest` | 2 | PlayerId, extensionObject |
| `Protos.RemoveMemberResponse` | 1 | extensionObject |
| `Protos.RemovePurchaseContextRequest` | 2 | PurchaseContextId, extensionObject |
| `Protos.RemovePurchaseContextResponse` | 1 | extensionObject |
| `Protos.RemoveStoreItemRecipientBindingRequest` | 3 | ItemId, RecipientId, extensionObject |
| `Protos.RemoveStoreItemRecipientBindingResponse` | 1 | extensionObject |
| `Protos.RenameModLoadoutTabRequest` | 3 | TabId, TabName, extensionObject |
| `Protos.RenameModLoadoutTabResponse` | 1 | extensionObject |
| `Protos.RenameSquadPresetTabRequest` | 4 | CombatType, TabId, TabName, extensionObject |
| `Protos.RenameSquadPresetTabResponse` | 1 | extensionObject |
| `Protos.RepeatableMilestone` | 3 | PointInterval, Reward, extensionObject |
| `Protos.ReplaceConsumableRequest` | 4 | ExistingConsumableId, NewConsumableId, UnitToApplyTo, extensionObject |
| `Protos.ReplaceConsumableResponse` | 4 | _unitStamina, ExpireTimeInSeconds, Energy, extensionObject |
| `Protos.ReportDeprecatedLocKeysUsedRequest` | 2 | _usedDeprecatedKeys, extensionObject |
| `Protos.ReportDeprecatedLocKeysUsedResponse` | 1 | extensionObject |
| `Protos.ReportGuildRequest` | 5 | EventType, TerritoryMapId, ReportDetails, Category, extensionObject |
| `Protos.ReportGuildResponse` | 1 | extensionObject |
| `Protos.ReportOddsDiscloseRequest` | 2 | PackId, extensionObject |
| `Protos.ReportOddsDisclosureResponse` | 1 | extensionObject |
| `Protos.ReportPlayerRequest` | 4 | PlayerId, EventReportType, Detail, extensionObject |
| `Protos.ReportPlayerResponse` | 1 | extensionObject |
| `Protos.ReportTelemetryRequest` | 3 | _telemetryEvent, CurrentClientTime, extensionObject |
| `Protos.ReportTelemetryResponse` | 1 | extensionObject |
| `Protos.RequestEnvelope` | 48 | CorrelationId, ServiceName, MethodName, Payload, AuthId, AuthToken, ClientVersion, ClientStartupTimestamp, Platform, Region, ClientExternalVersion, ClientInternalVersion, RequestId, AcceptEncoding, _flag, _telemetryEvent, CurrentClientTime, NimbleSessionId, Timezone, FirmwareVersion, Carrier, NetworkAccess, HardwareId, AdvertiserId, … (+24) |
| `Protos.Requirement` | 5 | EvalType, _requirementItem, Id, DescKey, extensionObject |
| `Protos.RequirementItem` | 8 | Type, Id, Value, LocKey, Negate, RosterUnitType, DateParam, extensionObject |
| `Protos.enum RequirementType` | 123 | value__, CURRENCYREQUIREMENT, ROSTERCHARACTERNOTCATEGORYNOTINSQUADREQUIREMENT, SQUADSIZEMAXIMUMREQUIREMENT, ROSTERCHARACTERCATEGORYNOTINUSEREQUIREMENT, ROSTERCHARACTERNOTINUSEREQUIREMENT, SQUADSIZEMINIMUMREQUIREMENT, ENEMYTHREATLEVELREQUIREMENT, ANYCHARACTERREADYFORPROMOTION, UNITTRAININGAVAILABLE, PLAYERLEVELMINREQUIREMENT, ROSTERUNITTYPEREQUIREMENT, ROSTERUNITTYPEMAXREQUIREMENT, UNITABILITYUPGRADEAVAILABLE, UNITGEAREQUIPAVAILABLE, CURRENTUNITGEAREQUIPAVAILABLE, PLAYERUNITHEALTHBELOWPERCENT, REQUIREMENTINVALID, CURRENTCHARACTERREADYFORPROMOTION, BATTLEANYSTATUSEFFECTACTIVE, STOREITEMAVAILABLEFORPURCHASE, CURRENTMISSIONHASGEARDROP, PLAYERLEVELMAXREQUIREMENT, CAMPAIGNREQUIREMENT, … (+99) |
| `Protos.RerollDatacronAffixRequest` | 3 | DatacronId, Tier, extensionObject |
| `Protos.RerollDatacronAffixResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.RerollModRequest` | 3 | ModId, ChosenRerollUnitStatId, extensionObject |
| `Protos.RerollModResponse` | 3 | StatMod, InventoryDelta, extensionObject |
| `Protos.ResetDailyActionCapRequest` | 2 | Id, extensionObject |
| `Protos.ResetDailyActionCapResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.ResetWarRequest` | 2 | _warId, extensionObject |
| `Protos.ResetWarResponse` | 3 | InventoryDelta, _war, extensionObject |
| `Protos.enum ResponseCode` | 38 | value__, NORESPONSECODE, OK, ERROR, SERVERERROR, SESSIONEXPIRED, AUTHFAILED, RATEEXCEEDED, SERVERUNAVAILABLE, INVALIDREQUEST, INVALIDDATA, LEADERBOARDMATCHMAKINGERROR, UNAUTHORIZED, SUSPENDED, SERVEROUTAGE, FEATURESUSPENDED, MISSINGPAYLOAD, NOCONTENT, NETWORKUNAVAILABLE, SEQUENCEHIGH, SEQUENCELOW, RECORDNOTFOUND, EVENTNOTFOUND, INSUFFICIENTRESOURCES, … (+14) |
| `Protos.ResponseEnvelope` | 12 | CurrentServerTime, Payload, Code, Message, ContentEncoding, StackTrace, _dynamicMessage, MaintenanceMessage, MaintenanceLink, SubCode, NextRequestToken, extensionObject |
| `Protos.ReturningStatus` | 4 | LapsedDurationDays, WelcomeBackDefinitionId, CreationTime, extensionObject |
| `Protos.RevealAllSecondariesRequest` | 3 | _modId, ModSessionId, extensionObject |
| `Protos.RevealAllSecondariesResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.RevokeGuildInviteRequest` | 2 | PlayerId, extensionObject |
| `Protos.RevokeGuildInviteResponse` | 3 | _status, Profile, extensionObject |
| `Protos.RiverEvent` | 5 | Type, Step, Timestamp, _parameter, extensionObject |
| `Protos.RiverEventParameter` | 3 | KeyType, Value, extensionObject |
| `Protos.RiverRequest` | 28 | SynergyId, PlayerId, _event, BuildId, ServerSessionId, SellId, NimbleSessionId, Platform, Timezone, FirmwareVersion, Carrier, NetworkAccess, HardwareId, AdvertiserId, VendorId, AndroidId, JailbrokenFlag, PiracyFlag, ClientIpAddress, CountryCode, DeviceId, Application, AppTransparencyTracking, AndroidAppSetId, … (+4) |
| `Protos.RoomData` | 7 | RoomId, Type, Name, Topic, CreationTime, OwnerId, extensionObject |
| `Protos.enum RoomType` | 6 | value__, GUILDDEFAULT, GUILDCHAT, GUILDCHATOFFICER, PLAYERDIRECTCHAT, PLAYERGROUPCHAT |
| `Protos.Roster` | 4 | _unit, _squad, _shipSquad, extensionObject |
| `Protos.enum RosterUnitType` | 4 | value__, ROSTERLEGACY, ROSTERERA, ROSTERALL |
| `Protos.RpcEndpoint` | 5 | ServiceName, MethodName, Endpoint, RequestMethod, extensionObject |
| `Protos.enum RpcHttpRequestMethod` | 3 | value__, HTTPPOST, HTTPGET |
| `Protos.SaveModLoadoutRequest` | 3 | TabId, _modLoadout, extensionObject |
| `Protos.SaveModLoadoutResponse` | 1 | extensionObject |
| `Protos.SavePurchaseContextRequest` | 5 | StorePlatform, ProductId, StoreItemId, _item, extensionObject |
| `Protos.SavePurchaseContextResponse` | 2 | PurchaseContextId, extensionObject |
| `Protos.SaveSquadConfig` | 4 | Id, DescKey, TournamentMapDefinitionId, extensionObject |
| `Protos.SaveSquadPresetRequest` | 4 | CombatType, TabId, _updatedSquadPreset, extensionObject |
| `Protos.SaveSquadPresetResponse` | 1 | extensionObject |
| `Protos.SaveSquadRequest` | 4 | PlayerSquad, EventId, InstanceId, extensionObject |
| `Protos.SaveSquadResponse` | 4 | SquadExpireTime, _inventoryDelta, SquadSaveTime, extensionObject |
| `Protos.SaveTerritoryDefenseRequest` | 2 | Defenses, extensionObject |
| `Protos.SaveTerritoryDefenseResponse` | 1 | extensionObject |
| `Protos.SaveTerritoryTournamentRosterRequest` | 2 | TerritoryMapId, extensionObject |
| `Protos.SaveTerritoryTournamentRosterResponse` | 3 | _tournamentRoster, _datacronRoster, extensionObject |
| `Protos.ScaleItemModifier` | 3 | Scale, _target, extensionObject |
| `Protos.enum ScheduleInstanceFilter` | 3 | value__, VISIBLE, ACTIVE |
| `Protos.SearchCriteria` | 8 | MinMemberCount, MaxMemberCount, IncludeInviteOnly, AllyOnly, MinGuildGalacticPower, MaxGuildGalacticPower, _recentTbParticipatedIn, extensionObject |
| `Protos.SeasonDefinition` | 8 | Id, NameKey, _seasonLeagueReference, _seasonDivisionReference, SeasonRewardTableId, MinimumBannerSeasonRewardThreshold, _divisionSkillBoundary, extensionObject |
| `Protos.SeasonDivisionDefinition` | 4 | Id, NameKey, MinPower, extensionObject |
| `Protos.SeasonDivisionReference` | 2 | Id, extensionObject |
| `Protos.SeasonDivisionSkillBoundary` | 4 | LeagueId, DivisionId, MinSkill, extensionObject |
| `Protos.SeasonLeagueDefinition` | 7 | Id, NameKey, Icon, Rank, Sprite, PortraitFrame, extensionObject |
| `Protos.enum SeasonLeaguePromoteSystem` | 3 | value__, SEASONLEAGUEPROMOTENONE, SEASONLEAGUEPROMOTESEASONPOINTS |
| `Protos.SeasonLeagueReference` | 6 | DefinitionId, PromoteSystem, PromoteCondition, _rewardPreview, PromoteConditionTable, extensionObject |
| `Protos.SeasonRewardTable` | 3 | Id, _row, extensionObject |
| `Protos.SeasonRewardTableKey` | 4 | League, Division, Rank, extensionObject |
| `Protos.SeasonRewardTableRow` | 4 | Key, _primaryReward, _detailedReward, extensionObject |
| `Protos.SeasonStatus` | 12 | SeasonId, EventInstanceId, League, Wins, Losses, SeasonPoints, Division, JoinTime, EndTime, Remove, Rank, extensionObject |
| `Protos.SeasonStatusRemoval` | 3 | SeasonId, EventInstanceId, extensionObject |
| `Protos.SeasonStatusUpdate` | 3 | _seasonStatus, _seasonStatusRemoval, extensionObject |
| `Protos.SeasonStatusUpdateWrapper` | 2 | _seasonStatusUpdate, extensionObject |
| `Protos.SegmentedContent` | 2 | _segments, extensionObject |
| `Protos.SegmentedContentDetailsRequest` | 3 | AcceptLanguage, ContentNameSpace, extensionObject |
| `Protos.SegmentedContentDetailsResponse` | 2 | Content, extensionObject |
| `Protos.SelectArtifactRequest` | 3 | NodeIdentifier, SelectedIndex, extensionObject |
| `Protos.SelectArtifactResponse` | 3 | Artifact, NodeSelection, extensionObject |
| `Protos.SelectMerchantNodeRequest` | 2 | NodeIdentifier, extensionObject |
| `Protos.SelectMerchantNodeResponse` | 2 | NodeIdentifier, extensionObject |
| `Protos.SellItem` | 4 | Type, Id, Quantity, extensionObject |
| `Protos.SellItemRequest` | 2 | _item, extensionObject |
| `Protos.SellItemResponse` | 2 | _inventoryDelta, extensionObject |
| `Protos.SellModRequest` | 2 | _modId, extensionObject |
| `Protos.SellModResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.ServerProperty` | 3 | Key, Value, extensionObject |
| `Protos.enum SessionStartContext` | 5 | value__, PUSHNOTE, NORMALAPPSTART, RESUME, RESTART |
| `Protos.SessionUpdate` | 4 | AuthId, _added, _removed, extensionObject |
| `Protos.SetInboxWhitelistRequest` | 2 | Category, extensionObject |
| `Protos.SetInboxWhitelistResponse` | 1 | extensionObject |
| `Protos.SetPlayerPortraitRequest` | 2 | SelectedPlayerPortraitId, extensionObject |
| `Protos.SetPlayerPortraitResponse` | 1 | extensionObject |
| `Protos.SetPlayerPrefRequest` | 2 | _entries, extensionObject |
| `Protos.SetPlayerPrefResponse` | 1 | extensionObject |
| `Protos.SetPlayerTitleRequest` | 2 | SelectedPlayerTitleId, extensionObject |
| `Protos.SetPlayerTitleResponse` | 1 | extensionObject |
| `Protos.Shipment` | 5 | ShipmentType, _shipmentItem, NextRefreshTime, DailyRefreshCapKey, extensionObject |
| `Protos.ShipmentItem` | 10 | ItemId, PreviewItem, PurchaseCost, StartTime, EndTime, Purchased, PurchaseRequirementId, PurchaseRequirementBadgeTexture, PurchaseRequirementBadgePermanent, extensionObject |
| `Protos.enum ShipmentRefreshType` | 3 | value__, LOCAL, GLOBAL |
| `Protos.ShipmentTransactionRequest` | 5 | ShipmentType, ItemId, FromFindFlow, SubLocation, extensionObject |
| `Protos.ShipmentTransactionResponse` | 2 | PurchasedResult, extensionObject |
| `Protos.enum ShipmentType` | 17 | value__, SHIPMENTPVP, SHIPMENTPVE, SHIPMENTSHIPWAR, SHIPMENTCANTINA, SHIPMENTSHIPPVP, SHIPMENTSHARD, SHIPMENTGUILD, SHIPMENTMOD, SHIPMENTTERRITORY, SHIPMENTPREMIUM, SHIPMENTSEASONS, SHIPMENTCONQUEST, SHIPMENTDATACRON, SHIPMENTGALACTICLEGENDS, SHIPMENTEPISODE, SHIPMENTERA |
| `Protos.SingularCampaignRequest` | 4 | Deferred, PassthroughParam, DeeplinkData, extensionObject |
| `Protos.SingularCampaignResponse` | 2 | Status, extensionObject |
| `Protos.enum SingularCampaignStatus` | 8 | value__, NONE, ALREADYCLAIMED, INACTIVE, INELIGIBLE, INVALIDDATA, REQUIREMENTSNOTMET, SUCCESS |
| `Protos.Skill` | 3 | Id, Tier, extensionObject |
| `Protos.SkillDefinition` | 9 | Id, NameKey, IconKey, _tier, AbilityReference, SkillType, IsZeta, OmicronMode, extensionObject |
| `Protos.SkillDefinitionReference` | 5 | SkillId, RequiredTier, RequiredRarity, RequiredRelicTier, extensionObject |
| `Protos.SkillTierDefinition` | 9 | RecipeId, RequiredUnitLevel, RequiredUnitRarity, RequiredUnitTier, PowerOverrideTag, RequiredUnitRelicTier, IsZetaTier, IsOmicronTier, extensionObject |
| `Protos.enum SkillType` | 3 | value__, UNITSKILL, CREWSKILL |
| `Protos.SkillUpgrade` | 4 | Id, Tier, Unlocked, extensionObject |
| `Protos.SkipTutorialRequest` | 1 | extensionObject |
| `Protos.SkipTutorialResponse` | 1 | extensionObject |
| `Protos.SliceABundleOfferTray` | 4 | Id, _trayContents, StoreItemOfferId, extensionObject |
| `Protos.SliceABundleTableOfferEntry` | 4 | TraySelectionKey, OfferId, _additionalBucketItems, extensionObject |
| `Protos.SlotEquipmentRequest` | 3 | UnitId, _equipment, extensionObject |
| `Protos.SlotEquipmentResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.SocialStatus` | 6 | _chatIcons, _chatTitles, _guildLogo, _guildColor, _chatPortraits, extensionObject |
| `Protos.SocialStatusGuildBannerColor` | 3 | Id, GuildBannerColorId, extensionObject |
| `Protos.SocialStatusGuildBannerLogo` | 3 | Id, GuildBannerLogoId, extensionObject |
| `Protos.SocialStatusIcon` | 3 | Id, Texture, extensionObject |
| `Protos.SocialStatusPortrait` | 3 | Id, PlayerPortraitId, extensionObject |
| `Protos.SocialStatusTitle` | 3 | Id, PlayerTitleId, extensionObject |
| `Protos.enum SpecialValue` | 4 | value__, NONE, MOSTPOPULAR, BESTVALUE |
| `Protos.Squad` | 9 | _cell, TargetingTactic, SquadType, TargetingSetId, ExpireTime, LastSaveTime, SupportInheritFromDefinitionId, Datacron, extensionObject |
| `Protos.SquadCell` | 14 | UnitId, UnitDefId, CellIndex, UnitBattleStat, MessageReticle, ProgressItem, SquadUnitType, _crewBattleStat, UnitState, Selectable, OverkillItem, InheritFromDefinitionId, ForcedUnitEraLevel, extensionObject |
| `Protos.SquadPreset` | 4 | Index, _unitId, Name, extensionObject |
| `Protos.SquadPresetTab` | 5 | Id, Name, _squadPreset, Unique, extensionObject |
| `Protos.enum SquadType` | 59 | value__, SQUADNEUTRAL, SQUADLIGHT, SQUADDARK, SQUADEVENT, SQUADPVP, SQUADRESTRICTED, SQUADSUPPLIED, SQUADWAR, SQUADEVENT01, SQUADEVENT02, SQUADEVENT03, SQUADEVENT04, SQUADEVENT05, SQUADEVENT06, SQUADEVENT07, SQUADEVENT08, SQUADEVENT09, SQUADEVENT10, SQUADEVENT11, SQUADEVENT12, SQUADEVENT13, SQUADEVENT14, SQUADEVENT15, … (+35) |
| `Protos.enum SquadUnitType` | 6 | value__, UNITTYPEDEFAULT, UNITTYPELEADER, UNITTYPECOMMANDER, UNITTYPESUPPORT, UNITTYPEREINFORCEMENT |
| `Protos.StarterGuildDef` | 10 | GuildType, NameKey, _allowedRaidId, MessageKey, _allowedRaidCampaignId, BannerLogoId, BannerColorId, ExternalMessageKey, SubNameKey, extensionObject |
| `Protos.Stat` | 7 | DefaultHash1, DefaultHash2, UnitStatId, StatValueDecimal, UnscaledDecimalValue, UiDisplayOverrideValue, Scalar |
| `Protos.StatDef` | 2 | _stat, extensionObject |
| `Protos.StatMod` | 15 | Id, DefinitionId, Level, Tier, SellValue, RemoveCost, Locked, PrimaryStat, _secondaryStat, Xp, LevelCost, BonusQuantity, ConvertedItem, RerolledCount, extensionObject |
| `Protos.StatModDefinition` | 17 | Id, Slot, SetId, Rarity, NameKey, DescKey, _missionLookup, _raidLookup, _actionLinkLookup, LevelTableId, PromotionId, PromotionRecipeId, TierUpRecipeTableId, OverclockRecipeTableId, RerollCapTableId, _raidImmediateLookup, extensionObject |
| `Protos.StatModFilter` | 16 | Id, Name, OnlyUnassignedMods, OnlyModsNotInLoadouts, _slot, _modSetId, _primaryStat, FilterOnAllSecondariesRevealed, FilterOnNoSecondariesRevealed, _secondaryStatConfig, LevelComparison, _rarity, _tier, SortMethod, InvertSort, extensionObject |
| `Protos.StatModSecondaryStatFilterConfig` | 5 | Include, SecondaryStat, StackComparison, RollQualityComparison, extensionObject |
| `Protos.StatModSetBonus` | 3 | _abilityId, Stat, extensionObject |
| `Protos.StatModSetDefinition` | 8 | Id, Name, Icon, CompleteBonus, MaxLevelBonus, SetCount, OverclockBonus, extensionObject |
| `Protos.enum StatModSlot` | 7 | value__, STATMODSLOT01, STATMODSLOT02, STATMODSLOT03, STATMODSLOT04, STATMODSLOT05, STATMODSLOT06 |
| `Protos.StatModStat` | 7 | Stat, _roll, StatRolls, StatRollerBoundsMin, StatRollerBoundsMax, _unscaledRollValue, extensionObject |
| `Protos.enum StatModTier` | 6 | value__, STATMODTIER01, STATMODTIER02, STATMODTIER03, STATMODTIER04, STATMODTIER05 |
| `Protos.StatModUpdaterDefinition` | 6 | Id, Type, TableId, MinIncrementValueDecimal, MaxIncrementValueDecimal, extensionObject |
| `Protos.enum StatModUpdaterType` | 3 | value__, UPDATETYPESTATTABLE, UPDATETYPESTATROLL |
| `Protos.StatProgression` | 4 | Id, Stat, Operation, extensionObject |
| `Protos.StatValueRange` | 5 | Stat, BattleStat, Min, Max, extensionObject |
| `Protos.StatValueRangeNumber` | 3 | Value, Inclusive, extensionObject |
| `Protos.Store` | 25 | _storeItem, StoreVersion, _shipment, _immediatePurchaseStoreItems, _webStoreItemsShared, _currencyUpdate, CountryCode, PackOddsUrlPrefix, _storefrontCountriesMapping, PlayerId, StoreElements, PremiumCurrencyExpirationDays, _cascadingStoreOffer, StoreBannerType, BannerSettings, _couponBinding, ConquestEventDetails, EpisodeDetails, ConquestDetails, IsCountryExcludedForWebStoreMtx, IsUnderage, _sliceABundleStoreOffer, _webStoreConfig, _storeItemOffer, … (+1) |
| `Protos.StoreBannerSettings` | 4 | LinkAccount, LinkAccountIncentivized, OpenWebStore, extensionObject |
| `Protos.enum StoreBannerType` | 5 | value__, BANNERNONE, BANNERLINKACCOUNT, BANNERLINKACCOUNTINCENTIVIZED, BANNERWEBSTORE |
| `Protos.StoreCouponBinding` | 4 | CouponId, _storeOfferId, ExpirationTime, extensionObject |
| `Protos.StoreDynamicUpdate` | 6 | _updatedStoreItem, _removedStoreItem, _subscriptionStatus, _removedSubscription, _shipment, extensionObject |
| `Protos.StoreElements` | 2 | _heroBanner, extensionObject |
| `Protos.StoreItem` | 58 | _id, _offer, Order, Quantity, MaxPurchasePerTransaction, PurchaseLimit, UiIcon, Filter, StartTime, EndTime, SpecialValue, RarityPreview, NameKey, DescriptionKey, DetailedDescriptionKey, PromoText1Key, PromoText2Key, PromoBackground, CarouselTextKey, RarityMaxPreview, BonusQuantity, ShowBuyConfirmation, ChanceAtTextKey, PackOpeningCrystalPrefab, … (+34) |
| `Protos.enum StoreItemCategory` | 11 | value__, SICCHARACTER, SICSHIP, SICRELIC, SICGEAR, SICMATERIAL, SICCURRENCY, SICCALENDAR, SICBATTLEPASS, SICMOD, SICDATACRON |
| `Protos.enum StoreItemDetailsModalType` | 7 | value__, MODALDEFAULT, MODALA, MODALB, MODALC, MODALD, MODALDVARIANT |
| `Protos.StoreItemOfferMapEntry` | 3 | Id, Offer, extensionObject |
| `Protos.StoreItemPreview` | 4 | MinLevel, MaxLevel, _item, extensionObject |
| `Protos.enum StoreItemType` | 7 | value__, STOREITEMTYPEPACK, STOREITEMTYPEPREMIUMCURRENCY, STOREITEMTYPEUPSIGHT, STOREITEMTYPEPOWERUPBUNDLES, STOREITEMTYPEWEBSTOREPACK, STOREITEMTYPESLICEABUNDLESEQUENTIALLYUNLOCKINGTRAYS |
| `Protos.StoreLocation` | 3 | StoreTab, Order, extensionObject |
| `Protos.enum StorePlatform` | 8 | value__, UNSETSTOREPLATFORM, APPLE, GOOGLE, DEV, FB, WEB, EAAPP |
| `Protos.StorePromoDialog` | 5 | StoreItemId, DescriptionTexture, ButtonKey, ShowPrice, extensionObject |
| `Protos.StoreReceipt` | 6 | TransactionId, _receipt, Signature, ProductId, StoreVersion, extensionObject |
| `Protos.StoreReceiptQueue` | 2 | _receipts, extensionObject |
| `Protos.StoreRequest` | 6 | AcceptLanguage, CountryCode, CurrencyCode, ClientIpAddress, IncludeCouponDetails, extensionObject |
| `Protos.StoreResponse` | 2 | Store, extensionObject |
| `Protos.enum StoreTab` | 14 | value__, PACK, PREMIUMCURRENCY, UPSIGHT, POWERUPBUNDLES, SHIPPACK, STORETAB2, UNITGUIDE, CONQUESTPASS, GIFTING, EPISODEPASSTAB, ERATAB, MARQUEETAB, PERSONALTAB |
| `Protos.StoreTransactionRecipientChoiceOption` | 3 | StoreItemId, _recipientChoiceId, extensionObject |
| `Protos.StoreTransactionRequest` | 16 | ItemId, PaymentCurrency, Receipt, Signature, StorePlatform, Price, CurrencyCode, Quantity, InboxReceipt, VisitReason, SubLocation, ClientIpAddress, CountryCode, Locale, PurchaseContextId, extensionObject |
| `Protos.StoreTransactionResponse` | 13 | _purchasedResult, AmountSpent, LastSpentTime, StoreItemNameKey, RedemptionCodeOffer, Player, RedemptionCodeMessageSubjectKey, RedemptionCodeMessageBodyKey, CartName, CheckoutCode, _unresolvedRecipientChoice, GameId, extensionObject |
| `Protos.StorefrontCountriesMappings` | 3 | Storefront, _countryCode, extensionObject |
| `Protos.Subscription` | 6 | Type, Duration, TopDescriptionKey, SuccessMessageKey, DetailDescriptionKey, extensionObject |
| `Protos.SubscriptionStatus` | 3 | ItemId, Remaining, extensionObject |
| `Protos.enum SubscriptionType` | 2 | value__, SUBSCRIPTIONDAILY |
| `Protos.SummonStatTable` | 3 | Rarity, StatTable, extensionObject |
| `Protos.SuspensionDetails` | 7 | SubjectKey, SubjectValue, DetailsKey, DetailsValue, SuspendedUntil, AllyCode, extensionObject |
| `Protos.SystemModifier` | 7 | Id, ScaleItemModifier, _location, AdditionalItemModifier, DescKey, ActionLink, extensionObject |
| `Protos.Table` | 3 | Id, _row, extensionObject |
| `Protos.TableRow` | 3 | Key, Value, extensionObject |
| `Protos.TagTier` | 3 | Tag, Tier, extensionObject |
| `Protos.TargetedStat` | 3 | Stat, TargetRuleId, extensionObject |
| `Protos.TargetingSet` | 3 | Id, _weight, extensionObject |
| `Protos.TargetingWeight` | 4 | Type, Param, ValueDecimal, extensionObject |
| `Protos.TerritoryBattleConflictZoneDefinition` | 12 | CombatType, _victoryPointRewards, ZoneDefinition, ConflictImage, TerritoryBattleZoneUnitType, AlternateChaseIcon, ForceAlignment, IsBonus, IsNewestBonusZone, FlipChaseIconX, FlipChaseIconY, extensionObject |
| `Protos.TerritoryBattleDefinition` | 30 | Id, NameKey, DescriptionKey, PrefabName, RoundDuration, RoundCount, _conflictZoneDefinition, _strikeZoneDefinition, _reconZoneDefinition, IconPrefab, _rankRewardPreview, DetailImage, _dynamicDescription, _covertZoneDefinition, _statCategory, TerritoryCategory, RoundTallyDuration, _grantedAbilities, _localNotifications, MinMemberLevelForCommandingGuild, CommandNotificationCooldownDuration, MinimapPrefab, SearchImage, InfoImage, … (+6) |
| `Protos.TerritoryBattleOverviewData` | 11 | InstanceId, _conflictZoneStatus, CurrentRound, StartTime, EndTime, DisplayEndTime, TotalStars, MapId, RoundEndTime, HasUndeployedUnits, extensionObject |
| `Protos.TerritoryBattlePhaseStats` | 4 | _availableMissionCount, _availableCombatWaves, _covertMissionIds, extensionObject |
| `Protos.TerritoryBattlePlayerStatus` | 4 | _zoneStatus, _unitStatus, _datacronStatus, extensionObject |
| `Protos.TerritoryBattleResult` | 7 | InstanceId, DefinitionId, EndTime, TotalStars, _finalStat, StartTime, extensionObject |
| `Protos.TerritoryBattleStatus` | 15 | InstanceId, DefinitionId, _conflictZoneStatus, _strikeZoneStatus, _reconZoneStatus, PlayerStatus, CurrentRound, CurrentRoundEndTime, ChannelId, _covertZoneStatus, _currentStat, LastCommandNotificationTime, MapCompletedEarly, Selected, extensionObject |
| `Protos.enum TerritoryBattleZoneUnitType` | 5 | value__, CHARACTERZONE, SHIPZONE, MIXEDZONE, UNDEFINEDZONE |
| `Protos.TerritoryBonusZoneDefinition` | 3 | ZoneDefinition, LinkedBonusConflictId, extensionObject |
| `Protos.TerritoryCategoryBonus` | 4 | Category, MinGalacticScore, GalacticScoreMultiplier, extensionObject |
| `Protos.TerritoryConflictZoneActivityData` | 2 | ZoneData, extensionObject |
| `Protos.TerritoryConflictZoneStatus` | 2 | ZoneStatus, extensionObject |
| `Protos.enum TerritoryCovertMissionResult` | 4 | value__, UNATTEMPTED, ATTEMPTEDFAILED, ATTEMPTEDSUCCEEDED |
| `Protos.TerritoryCovertZoneActivityData` | 2 | ZoneData, extensionObject |
| `Protos.TerritoryCovertZoneDefinition` | 6 | ZoneDefinition, CampaignElementIdentifier, _victoryReward, DisableRewardOnMap, CombatType, extensionObject |
| `Protos.TerritoryCovertZoneStatus` | 4 | ZoneStatus, PlayersParticipated, SuccessfulAttempts, extensionObject |
| `Protos.TerritoryDefense` | 3 | SavedSquadConfigId, _zoneDefense, extensionObject |
| `Protos.TerritoryDefenseUpdate` | 3 | SavedSquadConfigId, _zoneDefenses, extensionObject |
| `Protos.TerritoryGuildOverviewProfile` | 5 | Id, Name, BannerColorId, BannerLogoId, extensionObject |
| `Protos.TerritoryGuildProfile` | 7 | Id, Name, LogoBackground, BannerColorId, BannerLogoId, GuildGalacticPower, extensionObject |
| `Protos.TerritoryLocalNote` | 8 | MessageKey, TitleKey, OptedIn, Phase, PhaseTimeOffsetSeconds, FinalMatchOnly, PerSubPhaseRoundEnd, extensionObject |
| `Protos.TerritoryMapActivityData` | 2 | MapData, extensionObject |
| `Protos.TerritoryMapData` | 5 | MapId, InstanceId, ActivityLogMessage, ForceRefresh, extensionObject |
| `Protos.enum TerritoryMapPhase` | 7 | value__, TERRITORYMAPOPTIN, TERRITORYMAPMATCHMAKING, TERRITORYMAPSETINITIALDEFENSE, TERRITORYMAPATTACK, TERRITORYMAPREVIEW, TERRITORYMAPEXPIRED |
| `Protos.TerritoryMapStat` | 3 | MapStatId, _playerStat, extensionObject |
| `Protos.TerritoryMapStatCategory` | 3 | Id, NameKey, extensionObject |
| `Protos.TerritoryMapStatContext` | 2 | _territoryBattlePhaseStats, extensionObject |
| `Protos.TerritoryPlayerDatacronStatus` | 3 | DatacronId, DeployedZoneId, extensionObject |
| `Protos.TerritoryPlayerStat` | 4 | MemberId, Score, _covertZoneResult, extensionObject |
| `Protos.TerritoryPlayerUnitStatus` | 3 | UnitId, DeployedZoneId, extensionObject |
| `Protos.TerritoryPlayerZoneStatus` | 3 | ZoneId, Attempt, extensionObject |
| `Protos.TerritoryReconPlatoonDefinition` | 5 | Id, NameKey, _squad, Reward, extensionObject |
| `Protos.TerritoryReconSquadDefinition` | 3 | Id, NameKey, extensionObject |
| `Protos.TerritoryReconZoneActivityData` | 4 | ZoneData, PlatoonId, _units, extensionObject |
| `Protos.TerritoryReconZoneDefinition` | 16 | ZoneDefinition, _platoonDefinition, AbilityImage, AbilityShortDescKey, AbilityLongDescKey, GoalDescKey, GoalImage, UnitRarity, RewardDescKey, SubTitleKey, _grantedAbilities, _strategicAbilities, CombatType, TerritoryBattleZoneUnitType, UnitRelicTier, extensionObject |
| `Protos.TerritoryReconZonePlatoon` | 3 | Id, _squad, extensionObject |
| `Protos.TerritoryReconZoneSquad` | 3 | Id, _unit, extensionObject |
| `Protos.TerritoryReconZoneStatus` | 3 | ZoneStatus, _platoon, extensionObject |
| `Protos.TerritoryReconZoneUnit` | 6 | UnitIdentifier, Level, MemberId, Tier, UnitRelicTier, extensionObject |
| `Protos.TerritoryReconZoneUnitData` | 6 | SquadId, UnitIdentifier, Level, Tier, UnitRelicTier, extensionObject |
| `Protos.TerritoryReward` | 4 | Type, Value, RewardId, extensionObject |
| `Protos.TerritoryRewardBracket` | 4 | VictoryPointReward, GalacticScoreRequirement, Reward, extensionObject |
| `Protos.enum TerritoryRewardStructure` | 3 | value__, TERRITORYREWARDPERSONAL, TERRITORYREWARDCUMULATIVE |
| `Protos.enum TerritoryRewardType` | 4 | value__, GALACTICSCORE, VICTORYPOINT, MYSTERYBOXCONFLICT |
| `Protos.TerritoryStatCovertZoneResult` | 3 | CovertZoneId, Result, extensionObject |
| `Protos.TerritoryStrikeZoneActivityData` | 2 | ZoneData, extensionObject |
| `Protos.TerritoryStrikeZoneDefinition` | 5 | CampaignElementIdentifier, ZoneDefinition, EncounterRewardTableId, CombatType, extensionObject |
| `Protos.TerritoryStrikeZoneStatus` | 3 | PlayersParticipated, ZoneStatus, extensionObject |
| `Protos.TerritoryTournamentConflictZoneDefinition` | 4 | CombatType, _victoryPointRewards, ZoneDefinition, extensionObject |
| `Protos.TerritoryTournamentConflictZoneStatus` | 2 | ZoneStatus, extensionObject |
| `Protos.TerritoryTournamentDailyRewardTable` | 2 | _row, extensionObject |
| `Protos.TerritoryTournamentDailyRewardTableRow` | 5 | League, Division, _primaryReward, _detailedReward, extensionObject |
| `Protos.TerritoryTournamentDefinition` | 29 | Id, NameKey, DescriptionKey, PrefabName, IconPrefab, DetailImage, TerritoryCategory, MinimapPrefab, RoundCount, RoundTallyDuration, MatchmakingDuration, _dynamicDescription, _completeRankRewardPreview, _grantedAbilities, _localNotifications, RestrictSetupDuringAttackPhase, _conflictZoneDefinition, _duelZoneDefinition, NoJoinDuration, DefensiveCharacterDeployWarningThreshold, DefensiveShipDeployWarningThreshold, MatchCount, _roundRewardPreview, SeasonPointsPerBannerPercent, … (+5) |
| `Protos.TerritoryTournamentDivisionDefinition` | 4 | Id, NameKey, Icon, extensionObject |
| `Protos.TerritoryTournamentDuelZoneDefinition` | 10 | ZoneDefinition, DuelImage, DuelRequirement, DuelDetail, EntryCategoryAllowed, DefenseEntryCategoryAllowed, _battleEnvironments, PointStructure, SaveSquadZoneNumber, extensionObject |
| `Protos.TerritoryTournamentDuelZoneStatus` | 7 | ZoneStatus, SquadCapacity, _warSquad, SquadCount, DefeatedSquadCount, ZoneAttemptCompleted, extensionObject |
| `Protos.TerritoryTournamentEventStatus` | 13 | TournamentEventId, _matchStatus, CurrentMatchId, CurrentMatchEndTime, Joined, HeatOffsetDuration, GroupId, PlayerStatus, SkillRating, League, Division, LastRosterSaveTime, extensionObject |
| `Protos.TerritoryTournamentLeagueDefinition` | 8 | Id, NameKey, Icon, Rank, Sprite, PortraitFrame, BlankIcon, extensionObject |
| `Protos.TerritoryTournamentMatchStatus` | 11 | TournamentMapId, HomeStatus, AwayStatus, CurrentRound, CurrentRoundEndTime, ChannelId, MatchId, MatchState, MatchStartTime, Opponent, extensionObject |
| `Protos.TerritoryTournamentParticipantStatus` | 4 | PlayerId, _conflictStatus, _duelStatus, extensionObject |
| `Protos.TerritoryTournamentPlayer` | 10 | Id, Name, TitleId, GuildName, GuildLogoId, GuildColorId, PortraitId, _seasonStatus, LeagueId, extensionObject |
| `Protos.TerritoryTournamentPlayerStatus` | 6 | _zoneStatus, _unitStatus, _tournamentRoster, _datacronRoster, _datacronStatus, extensionObject |
| `Protos.TerritoryWarBattleLogEvent` | 17 | Id, RangeKey, ExpireTimestamp, AttackerId, AttackerName, _attackerUnit, DefenderId, DefenderName, _defenderUnit, ZoneId, SquadId, StartTime, EndTime, Outcome, DefenseDatacron, AttackerDatacron, extensionObject |
| `Protos.TerritoryWarBattleLogUnitMetaData` | 14 | DefinitionId, Tier, Level, Power, Omegas, Zetas, SquadUnitType, StartingHealthPercent, StartingProtectionPercent, FinalHealthPercent, FinalProtectionPercent, RelicTier, Omicrons, extensionObject |
| `Protos.TerritoryWarConflictActivityData` | 3 | ZoneData, WarSquad, extensionObject |
| `Protos.TerritoryWarConflictZoneDefinition` | 14 | ZoneDefinition, CombatType, ConflictImage, ConflictRequirement, ConflictDetail, EntryCategoryAllowed, DefenseEntryCategoryAllowed, DeployDefensiveSquadReward, OffensiveWinReward, CompleteZoneReward, CompleteZoneRewardPerDefensiveSquad, _battleEnvironments, PointStructure, extensionObject |
| `Protos.TerritoryWarConflictZoneStatus` | 6 | ZoneStatus, SquadCapacity, _warSquad, SquadCount, DefeatedSquadCount, extensionObject |
| `Protos.TerritoryWarDefinition` | 40 | Id, NameKey, DescriptionKey, PrefabName, RoundCount, IconPrefab, DetailImage, RoundTallyDuration, _dynamicDescription, _firstRewardPreview, _secondRewardPreview, MatchmakingDuration, SetupDuration, _grantedAbilities, JoinEventActionCapKey, MinParticipants, TerrainPrefabName, MaxParticipants, MinScoreRequirement, _warConflictZoneDefinition, _reconZoneDefinition, _statCategory, RewardStructure, UniqueJoinId, … (+16) |
| `Protos.TerritoryWarGuildStatus` | 4 | Profile, _conflictStatus, _reconStatus, extensionObject |
| `Protos.TerritoryWarOverviewData` | 11 | Joined, DefensesDeployed, OptInEndTime, AttackPhaseEndTime, DisplayEndTime, MatchmakingDuration, SetInitialDefenseDuration, HomeGuild, AwayGuild, InstanceId, extensionObject |
| `Protos.TerritoryWarOverviewGuildData` | 6 | Profile, Score, _status, RoundAttempt, MapCompletedEarly, extensionObject |
| `Protos.TerritoryWarParticipant` | 5 | MemberId, JoinTime, Eligible, Power, extensionObject |
| `Protos.TerritoryWarPlayerStatus` | 5 | _unitStatus, _warRoster, _datacronRoster, _datacronStatus, extensionObject |
| `Protos.TerritoryWarPointStructure` | 12 | DeployDefensiveSquadReward, OffensiveWinReward, CompleteZoneReward, CompleteZoneRewardPerDefensiveSquad, _defendDefeatBonus, EmptySquadSlotBonus, SurvivingSquadUnitBonus, FullHealthUnitBonus, FullProtectionUnitBonus, FirstAttackBonus, EnemyDefeatedBonus, extensionObject |
| `Protos.TerritoryWarResult` | 8 | TerritoryWarId, Score, Power, OpponentScore, EndTimeSeconds, OpponentGuildProfile, StartTime, extensionObject |
| `Protos.TerritoryWarSquad` | 10 | PlayerId, PlayerName, Power, Squad, SquadId, SquadStatus, LockName, SuccessfulDefends, _crewInfo, extensionObject |
| `Protos.enum TerritoryWarSquadStatus` | 4 | value__, SQUADAVAILABLE, SQUADLOCKED, SQUADDEFEATED |
| `Protos.TerritoryWarStatus` | 16 | InstanceId, DefinitionId, CurrentRound, CurrentRoundEndTime, ChannelId, HomeGuild, AwayGuild, PlayerStatus, _currentStat, _optedInMemberId, _optedInMember, ExcludedFromWar, HasReportedEnemyGuild, LastCommandNotificationTime, MapCompletedEarly, extensionObject |
| `Protos.TerritoryWarZoneOverviewStatus` | 6 | ZoneId, ZoneState, SquadCapacity, UndefeatedSquadCount, SquadCount, extensionObject |
| `Protos.enum TerritoryZoneCommandState` | 4 | value__, NOCOMMAND, FOCUSED, IGNORED |
| `Protos.TerritoryZoneData` | 14 | ZoneId, ScoreDelta, ScoreTotal, ActivityLogMessage, State, MapId, MapInstanceId, ParticipationDelta, ParticipationTotal, SourceZoneId, InstanceType, GuildId, NotificationInstanceType, extensionObject |
| `Protos.TerritoryZoneDefinition` | 12 | ZoneId, PrefabName, NameKey, DescriptionKey, LinkedConflictId, MaxAttemptsAllowed, UnlockRequirement, MaxUnitCountPerPlayer, _grantedAbilities, DetailViewIconOverrideName, MapIconOverrideName, extensionObject |
| `Protos.enum TerritoryZoneInstance` | 4 | value__, ZONEINSTANCEBATTLE, ZONEINSTANCEHOME, ZONEINSTANCEAWAY |
| `Protos.TerritoryZoneOverviewStatus` | 3 | Id, ZoneState, extensionObject |
| `Protos.enum TerritoryZoneState` | 5 | value__, ZONELOCKED, ZONEACTIVE, ZONEOPEN, ZONECOMPLETED |
| `Protos.TerritoryZoneStatus` | 7 | ZoneId, ZoneState, Score, ChannelId, CommandMessage, CommandState, extensionObject |
| `Protos.enum ThreatLevel` | 6 | value__, NORMAL, ELITE, BOSS, ANY, SUMMON |
| `Protos.enum TickerAppearLocation` | 3 | value__, HOMEBASETICKER, STORETICKER |
| `Protos.TickerMessage` | 2 | _appearLocation, extensionObject |
| `Protos.TierModRequest` | 3 | ModId, ModSessionId, extensionObject |
| `Protos.TierModResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.TierUpUnitRequest` | 3 | UnitId, Tier, extensionObject |
| `Protos.TierUpUnitResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.TimeZoneChangeConfig` | 7 | AllowedChangesPerPeriod, AllowedChangesLocKey, AllowedChangesResetLocKey, MaxTimeZoneChangeHoursExclusive, _timeZoneDefinition, TimeZoneChangeCooldownId, extensionObject |
| `Protos.TimeZoneChangeRequest` | 2 | TimeZoneOffsetMinutes, extensionObject |
| `Protos.TimeZoneChangeResponse` | 1 | extensionObject |
| `Protos.TimeZoneChangeStatus` | 3 | ChangesRemaining, StatusExpirationMs, extensionObject |
| `Protos.TimeZoneDefinition` | 3 | TimeZoneOffsetMinutes, Primary, extensionObject |
| `Protos.TrackFtueStepRequest` | 4 | MessageGroupId, MessageStepId, Action, extensionObject |
| `Protos.TrackFtueStepResponse` | 1 | extensionObject |
| `Protos.TrainUnitRequest` | 3 | ParentId, _material, extensionObject |
| `Protos.TrainUnitResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.Unit` | 15 | Id, DefinitionId, CurrentRarity, CurrentLevel, CurrentXp, PromotionRecipeReference, _skill, UnitStat, _equipment, CurrentTier, _equippedStatModOld, _equippedStatMod, Relic, _purchasedAbilityId, extensionObject |
| `Protos.UnitBattleStat` | 20 | ActionCountMin, ActionCountMax, BattleStat, Level, Tier, _equipment, _skill, _statMod, UnitDefId, UnitId, UnitRelicTier, _purchasedAbilityId, OverrideAllStatsMultiplier, _overrideStatMultiplier, SkipStatsOverwrite, EnableLeaderAbility, ThreatLevelOverride, OverrideAllStatsTable, EraLevel, extensionObject |
| `Protos.enum UnitClass` | 3 | value__, UNITCLASSSTANDARD, UNITCLASSCOMMANDER |
| `Protos.UnitDef` | 62 | Id, UnitPrefab, ThumbnailName, NameKey, Rarity, MaxRarity, ForceAlignment, XpTableId, ActionCountMin, ActionCountMax, CombatType, DescKey, ThreatLevel, Obtainable, BaseId, PromotionRecipeReference, StatProgressionId, _categoryId, TrainingXpWorthBaseValueOverride, MaxLevelOverride, TrainingCostMultiplierOverride, CreationRecipeReference, _skillReference, BasePower, … (+38) |
| `Protos.UnitDefPreview` | 7 | Id, ThumbnailName, NameKey, BasicAttackRef, _limitBreakRef, _uniqueAbilityRef, extensionObject |
| `Protos.enum UnitDetailsAbilityLocation` | 5 | value__, ABILITYLOCATIONNORMAL, ABILITYLOCATIONADVANCED, ABILITYLOCATIONCREW, ABILITYLOCATIONHIDDEN |
| `Protos.enum UnitGuideActivityType` | 5 | value__, GUIDEACTIVITYEVENT, GUIDEACTIVITYRAID, GUIDEACTIVITYTERRITORYBATTLE, GUIDEACTIVITYUNDEFINED |
| `Protos.UnitGuideDefinition` | 25 | UnitBaseId, Type, ActivityId, DescriptionKey, RecommendationRequirementId, OverrideTexture, TitleKey, SubtitleKey, AdditionalActivationRequirementId, DisplayEnabled, ActionLinkImage, ActionLink, InPreview, HiddenUnitCount, PreviewArtImage, UpcomingEventKey, Featured, CampaignElementIdentifier, CharacterRecommendedEntryCategory, ShipRecommendedEntryCategory, GalacticLegend, BannerImage, BannerImageSubtextKey, UnitStagePrefab, … (+1) |
| `Protos.UnitGuideLayout` | 4 | Id, TabTitleKey, _layoutTier, extensionObject |
| `Protos.UnitGuideLayoutLine` | 2 | _unitGuideId, extensionObject |
| `Protos.UnitGuideLayoutTier` | 4 | TitleKey, SubtitleKey, _layoutLine, extensionObject |
| `Protos.enum UnitHealthState` | 4 | value__, UNITHEALTHSTATEANY, UNITHEALTHSTATEALIVE, UNITHEALTHSTATEDEAD |
| `Protos.UnitModRecommendation` | 3 | RecommendationSetId, UnitTier, extensionObject |
| `Protos.UnitPref` | 4 | UnitId, Favorite, UnitDefId, extensionObject |
| `Protos.UnitReplacement` | 3 | SourceUnitId, ReplacementUnitId, extensionObject |
| `Protos.UnitStaminaRequest` | 2 | _unitId, extensionObject |
| `Protos.UnitStaminaResponse` | 2 | _unitStamina, extensionObject |
| `Protos.enum UnitStat` | 62 | value__, UNITSTATMAXHEALTH, UNITSTATSTRENGTH, UNITSTATAGILITY, UNITSTATINTELLIGENCE, UNITSTATSPEED, UNITSTATATTACKDAMAGE, UNITSTATABILITYPOWER, UNITSTATARMOR, UNITSTATSUPPRESSION, UNITSTATARMORPENETRATION, UNITSTATSUPPRESSIONPENETRATION, UNITSTATDODGERATING, UNITSTATDEFLECTIONRATING, UNITSTATATTACKCRITICALRATING, UNITSTATABILITYCRITICALRATING, UNITSTATCRITICALDAMAGE, UNITSTATACCURACY, UNITSTATRESISTANCE, UNITSTATDODGEPERCENTADDITIVE, UNITSTATDEFLECTIONPERCENTADDITIVE, UNITSTATATTACKCRITICALPERCENTADDITIVE, UNITSTATABILITYCRITICALPERCENTADDITIVE, UNITSTATARMORPERCENTADDITIVE, … (+38) |
| `Protos.enum UnitTier` | 22 | value__, TIERUNDEFINED, TIER01, TIER02, TIER03, TIER04, TIER05, TIER06, TIER07, TIER08, TIER09, TIER10, TIER11, TIER12, TIER13, TIER14, TIER15, TIER16, TIER17, TIER18, TIER19, TIER20 |
| `Protos.UnitTierDef` | 4 | Tier, _equipmentSet, BaseStat, extensionObject |
| `Protos.UnitTierSummary` | 4 | BaseId, Tier, UnitRelicTier, extensionObject |
| `Protos.enum UnitType` | 5 | value__, UNITTYPESTANDARD, UNITTYPEXP, UNITTYPESELL, UNITTYPEPROMOTE |
| `Protos.UnitUpgrade` | 15 | Id, Level, Rarity, Tier, SkillTier, _skillOverride, RelicTier, BasicSkillTier, LeaderSkillTier, UniqueSkillTier, SpecialSkillTier, ExcludeZetaSkills, ExcludeOmicronSkills, _abilityDescriptiveTagSkillTier, extensionObject |
| `Protos.UnlockAnnouncementDefinition` | 5 | Feature, StartOffset, Interval, MessageKey, extensionObject |
| `Protos.UnlockModRequest` | 2 | ModId, extensionObject |
| `Protos.UnlockModResponse` | 1 | extensionObject |
| `Protos.UpdateAgeGateStatusRequest` | 3 | Age, DeviceId, extensionObject |
| `Protos.UpdateAgeGateStatusResponse` | 3 | AgeGateStatus, AllowPlatformLoginOverride, extensionObject |
| `Protos.UpdateArtifactRequest` | 3 | _equipArtifactId, _unequipArtifactId, extensionObject |
| `Protos.UpdateArtifactResponse` | 5 | _equippedArtifact, _unequippedArtifact, InventoryDelta, Player, extensionObject |
| `Protos.UpdateChannelSessionRequest` | 2 | _sessionUpdate, extensionObject |
| `Protos.UpdateChannelSessionResponse` | 1 | extensionObject |
| `Protos.UpdateDatacronPrefsRequest` | 3 | Id, Locked, extensionObject |
| `Protos.UpdateDatacronPrefsResponse` | 2 | Datacron, extensionObject |
| `Protos.UpdateEventFeatsRequest` | 2 | Id, extensionObject |
| `Protos.UpdateEventFeatsResponse` | 1 | extensionObject |
| `Protos.UpdateGuildRaidLaunchConfigRequest` | 2 | AutoLaunchConfig, extensionObject |
| `Protos.UpdateGuildRaidLaunchConfigResponse` | 2 | GuildProfile, extensionObject |
| `Protos.UpdateGuildRequest` | 2 | Profile, extensionObject |
| `Protos.UpdateGuildResponse` | 3 | Delta, Profile, extensionObject |
| `Protos.UpdateInboxRequest` | 5 | _messageId, Action, EntitlementId, MessageType, extensionObject |
| `Protos.UpdateInboxResponse` | 6 | _delta, Ally, Guild, Player, RoomJoined, extensionObject |
| `Protos.UpdateModLoadoutTabRequest` | 4 | TabId, TabName, Unique, extensionObject |
| `Protos.UpdateModLoadoutTabResponse` | 1 | extensionObject |
| `Protos.UpdateModsRequest` | 5 | UnitId, _modEquippedId, _modUnequippedId, AutoAssigned, extensionObject |
| `Protos.UpdateModsResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.UpdatePlayerNameRequest` | 2 | NewName, extensionObject |
| `Protos.UpdatePlayerNameResponse` | 3 | InventoryDelta, Player, extensionObject |
| `Protos.UpdatePlayerPreferenceRequest` | 5 | CommandMessages, _unitPref, _modRecommendationPref, _datacronFavoritePref, extensionObject |
| `Protos.UpdatePlayerPreferenceResponse` | 1 | extensionObject |
| `Protos.UpdateRoomRequest` | 5 | RoomId, Name, Topic, _addedPlayerId, extensionObject |
| `Protos.UpdateRoomResponse` | 3 | Room, _badInvite, extensionObject |
| `Protos.UpdateSquadPresetTabRequest` | 5 | CombatType, TabId, TabName, TabUnique, extensionObject |
| `Protos.UpdateSquadPresetTabResponse` | 1 | extensionObject |
| `Protos.UpdateUnitPrefRequest` | 2 | _unitPref, extensionObject |
| `Protos.UpdateUnitPrefResponse` | 1 | extensionObject |
| `Protos.UpgradeDatacronRequest` | 3 | DatacronId, TargetTier, extensionObject |
| `Protos.UpgradeDatacronResponse` | 2 | InventoryDelta, extensionObject |
| `Protos.UseConsumableRequest` | 3 | Id, UnitToApplyTo, extensionObject |
| `Protos.UseConsumableResponse` | 4 | _unitStamina, ExpireTimeInSeconds, Energy, extensionObject |
| `Protos.ValidateAccountLinkCodeRequest` | 2 | SourceLinkCode, extensionObject |
| `Protos.ValidateAccountLinkCodeResponse` | 4 | SourcePlayerLevel, SourcePlayerName, ExpireTime, extensionObject |
| `Protos.enum VfxAllocationLevel` | 4 | value__, VFXONDEMAND, VFXPRELOAD, VFXPRELOADANDALLOCATE |
| `Protos.Wallet` | 2 | _currencyItems, extensionObject |
| `Protos.War` | 8 | Id, _node, _warRoster, WarInvalid, WarLevel, SimAvailable, WarsCompleted, extensionObject |
| `Protos.WarBracketDef` | 4 | Min, Max, _node, extensionObject |
| `Protos.WarDef` | 7 | Id, Type, Difficulty, DailyResetCapKey, _node, _bracket, extensionObject |
| `Protos.WarNode` | 4 | Id, Status, WarOpponent, extensionObject |
| `Protos.WarNodeDef` | 5 | Id, Type, CombatType, Position, extensionObject |
| `Protos.enum WarNodeStatus` | 6 | value__, NODEHIDDEN, NODELOCKED, NODEUNLOCKED, NODECLAIMABLE, NODECOMPLETED |
| `Protos.WarNodeStatusUpdate` | 4 | NodeId, Status, NewOpponent, extensionObject |
| `Protos.enum WarNodeType` | 5 | value__, NODETYPEOPPONENT, NODETYPECHESTCOMMON, NODETYPECHESTRARE, NODETYPECHESTEPIC |
| `Protos.WarOpponent` | 6 | Id, PlayerName, Level, Squad, Power, extensionObject |
| `Protos.WarStatus` | 5 | WarId, _warRoster, _nodeStatus, WarsCompleted, extensionObject |
| `Protos.WebStoreAffix` | 3 | AffixIcon, AbilityDisplayName, extensionObject |
| `Protos.WebStoreBucket` | 3 | Id, _bucketCards, extensionObject |
| `Protos.WebStoreBucketItem` | 4 | Id, LocalizedQuantity, _dropRate, extensionObject |
| `Protos.WebStoreBucketItemShared` | 24 | Id, Image, LocalizedName, LocalizedDescription, Rarity, UnitTier, Mark, ForceAlignment, WebStoreItemType, ModSlot, ModTier, ModSetId, UnlockedAffixCount, MaxAffixCount, ExpirationTime, _factionAffixes, _alignmentAffixes, _characterAffixes, WebStoreGalacticBundle, _webStoreMysteryBoxItem, ArtifactPowerLevel, EffectIcon, _calendarDayRewards, extensionObject |
| `Protos.WebStoreBucketMapEntry` | 3 | StoreItemId, Bucket, extensionObject |
| `Protos.WebStoreCascadingOffer` | 5 | Title, Description, _webStoreLocation, WebStoreExclusive, extensionObject |
| `Protos.WebStoreConfig` | 3 | ConfigId, ConfigValue, extensionObject |
| `Protos.WebStoreGalacticBundle` | 6 | GalacticBundle, _unitUpgradeInfo, Info, _tabInfo, _inventoryItemInfo, extensionObject |
| `Protos.WebStoreGalacticBundleTabInfo` | 4 | LocalizedText, LocalizedTitle, LocalizedDescription, extensionObject |
| `Protos.WebStoreItem` | 25 | LocalizedName, LocalizedDescription, LocalizedDetailedDescription, LocalizedPromoText1, LocalizedPromoText2, LocalizedCarouselText, LocalizedChanceAtText, LocalizedGuarantee, _bucketItems, LocalizedQuantity, LocalizedBonusQuantity, LocalizedSpecialValue, WebStorePriceDiscountStyle, WebStorePromoTimerDisplay, WebStoreHideTimerThresholdDays, WebStoreShowPackOdds, Giftable, WebStoreExclusive, BucketId, DetailsModalType, LocalizedSimplifiedDescription, WebStoreShowFlare, _sliceABundleTableOffer, _additionalBucketItem, … (+1) |
| `Protos.enum WebStoreItemType` | 29 | value__, WSITUNKNOWN, WSITEQUIPMENT, WSITCURRENCY, WSITENERGY, WSITDATACRON, WSITCHARACTER, WSITLOGINCALENDAR, WSITSHIP, WSITMOD, WSITXP, WSITMODMATERIAL, WSITCHARACTERMATERIAL, WSITSHIPMATERIAL, WSITMATERIAL, WSITGALACTICBUNDLE, WSITMYSTERYBOX, WSITBATTLEPASS, WSITARTIFACT, WSITCONQUESTCONSUMABLE, WSITCONQUESTSTAMINAREGENREDUCE, WSITCONQUESTENERGYREGENREDUCE, WSITCONQUESTARTIFACTFREESWAPBONUS, WSITEPISODEPASS, … (+5) |
| `Protos.WebStoreLocation` | 3 | WebStoreTab, Order, extensionObject |
| `Protos.enum WebStorePackTier` | 5 | value__, WEBSTOREPACKTIERNONE, WEBSTOREPACKTIERBRONZIUM, WEBSTOREPACKTIERCHROMIUM, WEBSTOREPACKTIERAURODIUM |
| `Protos.enum WebStorePageContext` | 5 | value__, UNKNOWNPAGECONTEXT, WEBSTORE, LOYALTY, REDEEMCODE |
| `Protos.WebStorePageSwitchRequest` | 2 | PageContext, extensionObject |
| `Protos.WebStorePageSwitchResponse` | 1 | extensionObject |
| `Protos.WebStoreRedemptionCodeRequest` | 3 | RedeemCode, ClientIpAddress, extensionObject |
| `Protos.WebStoreRedemptionCodeResponse` | 3 | RedemptionMessage, RedemptionStatusCode, extensionObject |
| `Protos.enum WebStoreTab` | 13 | value__, WEBPACK, WEBPREMIUMCURRENCY, WEBSTORETAB2, WEBUNITGUIDE, WEBCONQUESTPASS, WEBYOUROFFERS, WEBLIMITEDTIMEOFFERS, WEBGIFTING, WEBEPISODEPASSTAB, WEBERATAB, WEBMARQUEETAB, WEBPERSONALTAB |
| `Protos.WebStoreUnitUpgradeInfo` | 8 | Id, Image, LocalizedName, LocalizedDescription, ForceAlignment, CombatType, UnitClass, extensionObject |
| `Protos.WebstoreSharedConstantRequest` | 2 | GamedataVersion, extensionObject |
| `Protos.WebstoreSharedConstantResponse` | 12 | _isResponseModified, _gamedataVersion, _config, EadpCheckoutUrl, EadpClientId, EadpClientBaseUrl, EadpEnv, TrackingMaterialEnabled, MtaApplicationId, MtaApplicationName, EadpGameId, extensionObject |
| `Protos.XpTable` | 3 | Id, _row, extensionObject |
| `Protos.XpTableRow` | 3 | Index, Xp, extensionObject |
| `Protos.YearMonth` | 3 | Year, Month, extensionObject |
| `Protos.ZoneDefenseUnitSquad` | 4 | _defensiveSquadUnit, DatacronId, PlayerAction, extensionObject |
| `Protos.ZoneDefenses` | 4 | SavedSquadZoneNumber, _defensiveSquad, SquadsAllowed, extensionObject |
| `Protos.ZoneDefensesUpdate` | 3 | SavedSquadZoneNumber, SquadsAllowed, extensionObject |
| `Rpc.enum HttpRequestMethod` | 3 | value__, POST, GET |
| `Rpc.RPCBase` | 31 | _client, _timeInfo, _coroutineCreator, _lifecycleController, _responseInterceptor, _responseNotifier, _telemetryDomainController, _playerDomainController, _serviceErrorHandlerProvider, _rpcContextTracker, _rpcRequestFactory, _frameworkEventDispatch, OnRPCCompleted, _requestId, _requestToken, _status, ResultBytes, _failureHandling, <ServiceName>k__BackingField, <MethodName>k__BackingField, <ServiceType>k__BackingField, <IsDone>k__BackingField, <ErrorCode>k__BackingField, <ErrorSubCode>k__BackingField, … (+7) |
| `Rpc.RPCContextTracker` | 1 | <CurrentRPCName>k__BackingField |
| `Rpc.enum RPCFailureHandling` | 4 | value__, PROMPT_RETRY, FORCE_RESTART, NONE |
| `Rpc.RPC`1` | 23 | MAX_BYTES_TO_LOG, _advertiserIdManager, _appTrackingTransparencyProvider, _notifier, _eadpSDKController, _rpcIdentifier, _serviceErrorHandler, _succeededCallback, _gameSuccessCallback, _gameFailCallback, _systemFailCallback, _gameCompleteCallback, _gameRetryCallback, _requestEnvelope, _deserializeResults, _serializedPayload, SendStartupTimestamp, _errorHandlerEnabled, _profileKey, result, cached, _authDO, _appWasPausedAndResumed |
| `Rpc.ResponseInterceptor` | 2 | _playerDCProvider, _interceptHandlers |
| `Rpc.ResponseNotifier` | 1 | _onGameServerRPCSuccess |
| `Rpc.RpcEndpoint` | 3 | _rpcIdentifier, _endpoint, _requestMethod |
| `Rpc.RpcEndpointRegistry` | 2 | _explicitEndpoints, _wildcardEndpoints |
| `Rpc.RpcEndpointTranslator` | 0 | — |
| `Rpc.RpcIdentifier` | 6 | WILDCARD_IDENTIFIER, _serviceName, _methodName, _serviceWildcard, _methodWildcard, _fullWildcard |
| `Rpc.RpcRequestFactory` | 7 | _rpcRequestMethodConverter, _rpcEndpointRegistry, _client, X_RPC_METHOD_NAME_HEADER, X_RPC_SERVICE_NAME_HEADER, COMMUNICATION_KEY_NAME_HEADER, _httpHeaders |
| `Rpc.RpcRequestMethodConverter` | 4 | PROTO_BINDING_FLAGS, QUERY_INITIAL_SEPARATOR, QUERY_SUBSEQUENT_SEPARATOR, _reflector |

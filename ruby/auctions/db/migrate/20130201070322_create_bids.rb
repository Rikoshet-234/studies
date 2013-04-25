class CreateBids < ActiveRecord::Migration
  def change
    create_table :bids do |t|
      t.integer :value
      t.belongs_to :user
      t.belongs_to :auction

      t.timestamps
    end
    add_index :bids, :user_id
    add_index :bids, :auction_id
  end
end
